#ifndef SIMULATOR_CONTROLER_H_
#define SIMULATOR_CONTROLER_H_

#include "common/thread-fn.h"
#include "simulator/taxi-data-queue.h"
#include "simulator/taxi-data-extraction.h"

#include <stack>
#include <string>
#include <curl/curl.h>

namespace mod {

class Controller : public common::ThreadFn {
 public:
  // return instance of Controller
  static Controller& Instance() {
    static Controller controller;
    return controller;
  }
  // start run
  void Run(int thread_num);

  // set/get host
  void set_host(const std::string& host) { host_ = host; }
  const std::string& host() const { return host_; }

  // set/get port
  void set_port(int port) { port_ = port; }
  int port() const { return port_; }

  // set/get taxi data path
  void set_taxi_data_path(const std::string& taxi_data_path) {
    taxi_data_path_ = taxi_data_path;
  }
  const std::string& taxi_data_path() const {
    return taxi_data_path_;
  }
  
 private:
  // producer and consumer
  static void ProduceTaxiData(int signo);
  static void ConsumeTaxiData(void* arg);

  // post http request
  void PostHttpRequest(const std::string& post_data);

  // fetch curl handle
  CURL* FetchCurlHandle();
  void ReleaseCurlHandle(CURL* handle);

  // controller construction
  Controller() : taxi_data_extraction_(NULL), prev_timestamp_(0) {
    pthread_mutex_init(&mutex_, NULL);
  };
  virtual ~Controller() { }

  // forbid copy operation
  Controller(const Controller& controller);
  Controller& operator=(const Controller& controller);

  // taxi data queue
  TaxiDataQueue taxi_data_queue_;
  // taxi data extraction
  TaxiDataExtraction* taxi_data_extraction_;
  // host of mod server
  std::string host_;
  // port of mod server
  int port_;
  // taxi data path
  std::string taxi_data_path_;
  // previous timestamp
  int64_t prev_timestamp_;
  // cache handles
  std::stack<CURL*> cache_handles_;
  // mutex for cache handles fetching
  pthread_mutex_t mutex_;
};

}  // namespace mod

#endif  // SIMULATOR_CONTROLER_H_
