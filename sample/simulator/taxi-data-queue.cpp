#include "simulator/taxi-data-queue.h"

namespace {
const size_t kDefaultMaxQueueSize = 6000000;
}

namespace mod {

TaxiDataQueue::TaxiDataQueue() : max_size_(kDefaultMaxQueueSize) {
  pthread_mutex_init(&mutex_,     NULL);
  pthread_cond_init(&cond_full_,  NULL);
  pthread_cond_init(&cond_empty_, NULL);
}

TaxiDataQueue::~TaxiDataQueue() {
}

void TaxiDataQueue::Enqueue(const TaxiData& taxi_data) {
  pthread_mutex_lock(&mutex_);
  while (taxi_datas_.size() >= max_size_) {
    pthread_cond_wait(&cond_full_, &mutex_);
  }
  taxi_datas_.push(taxi_data);
  pthread_cond_signal(&cond_empty_);
  pthread_mutex_unlock(&mutex_);
}

TaxiData TaxiDataQueue::Dequeue() {
  pthread_mutex_lock(&mutex_);
  while (taxi_datas_.empty()) {
    pthread_cond_wait(&cond_empty_, &mutex_);
  }
  TaxiData taxi_data = taxi_datas_.front();
  taxi_datas_.pop();
  pthread_cond_signal(&cond_full_);
  pthread_mutex_unlock(&mutex_);
  return taxi_data;
}

}  // namespace mod
