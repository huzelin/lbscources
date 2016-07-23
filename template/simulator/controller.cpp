#include "simulator/controller.h"
#include "common/log.h"
#include "proto/mod-update.pb.h"

#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

namespace mod {

void Controller::Run(int thread_num) {
  taxi_data_extraction_ = new TaxiDataExtraction(taxi_data_path_);
  for (int i = 0; i < thread_num; ++i) {
    Start(Controller::ConsumeTaxiData, this);
  }
  struct itimerval tick;
  signal(SIGALRM, Controller::ProduceTaxiData);
  memset(&tick, 0, sizeof(tick));

  tick.it_value.tv_sec     = 1;
  tick.it_value.tv_usec    = 0;
  tick.it_interval.tv_sec  = 1;
  tick.it_interval.tv_usec = 0;

  if (setitimer(ITIMER_REAL, &tick, NULL) < 0) {
    LOG_ERROR("set timer failed");
  } else {
    while (true) pause();
  }
}

void Controller::ProduceTaxiData(int signo) {
  Controller* controller = &(Controller::Instance());
  while (controller->taxi_data_extraction_->HasNext()) {  // is has next ?
    const TaxiData& taxi_data = controller->taxi_data_extraction_->taxi_data();
    int64_t timestamp = taxi_data.timestamp;
    if (controller->prev_timestamp_ == 0) {
      controller->taxi_data_queue_.Enqueue(taxi_data);
      controller->prev_timestamp_ = timestamp;
      break;
    } else if (controller->prev_timestamp_ + 1 >= timestamp) {
      controller->taxi_data_queue_.Enqueue(taxi_data);
    } else {
      controller->prev_timestamp_++;
      break;
    }
  }
}

void Controller::ConsumeTaxiData(void* arg) {
  Controller* controller = reinterpret_cast<Controller*>(arg);
  while (true) {
    TaxiData taxi_data = controller->taxi_data_queue_.Dequeue();
    LOG_DEBUG("start sending taxi data: %s %f %f %ld",
              taxi_data.id.c_str(), taxi_data.lon,
              taxi_data.lat, taxi_data.timestamp);
  
    ModUpdateRequest mod_update_request;
    mod_update_request.set_id(taxi_data.id);
    mod_update_request.set_lon(taxi_data.lon);
    mod_update_request.set_lat(taxi_data.lat);
    mod_update_request.set_speed(taxi_data.speed);
    mod_update_request.set_timestamp(taxi_data.timestamp);

    std::string post_data;
    mod_update_request.SerializeToString(&post_data);

    controller->PostHttpRequest(post_data);
  }
}

static size_t RecvData(void *buffer, size_t size, size_t nmemb, void *userp) {
  return size * nmemb;
}

void Controller::PostHttpRequest(const std::string& post_data) {
  CURL* handle = FetchCurlHandle();

  curl_easy_setopt(handle, CURLOPT_POSTFIELDS,    post_data.c_str());
  curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, post_data.size());

  CURLcode rc = curl_easy_perform(handle);
  if (rc != 0) {
    LOG_ERROR("curl error: %s", curl_easy_strerror(rc));
  }
  ReleaseCurlHandle(handle);
}

CURL* Controller::FetchCurlHandle() {
  CURL* handle = NULL;
  pthread_mutex_lock(&mutex_);
  if (cache_handles_.empty()) {
    handle = curl_easy_init();
    char url[512];
    snprintf(url, 512, "http://%s:%d/mod_update", host_.c_str(), port_);
    curl_easy_setopt(handle, CURLOPT_URL, url);

    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPIDLE,  120L);
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, RecvData);

    curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT_MS, 50);
    curl_easy_setopt(handle, CURLOPT_ACCEPTTIMEOUT_MS, 50);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 100);
  } else {
    handle = cache_handles_.top();
    cache_handles_.pop();
  }
  pthread_mutex_unlock(&mutex_);
  return handle;
}

void Controller::ReleaseCurlHandle(CURL* handle) {
  pthread_mutex_lock(&mutex_);
  cache_handles_.push(handle);
  pthread_mutex_unlock(&mutex_);
}

}  // namespace mod
