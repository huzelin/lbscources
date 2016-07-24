#ifndef SERVER_SERVER_H_
#define SERVER_SERVER_H_

#include <stdint.h>
#include <string>

#include "event.h"
#include "evhtp.h"

#include "server/grid/lbs_index.h"

namespace mod {

class Server {
 public:
  bool Start();

  void set_ip(const std::string& ip) { ip_ = ip; }
  const std::string& ip() const { return ip_; }

  void set_port(int port) { port_ = port; }
  int port() const { return port_; }

  void set_threads_num(int threads_num) { threads_num_ = threads_num; }
  int threads_num() const { return threads_num_; }

 private:
  // start http service for querying
  bool StartHttpServer();

  // process update/query
  static void ProcessUpdate(evhtp_request_t* request, void* arg);
  static void ProcessQuery(evhtp_request_t* request, void* arg);

  // process update/query implementation
  void ProcessUpdate(evhtp_request_t* request);
  void ProcessQuery(evhtp_request_t* request);

  // 检索结果拼装
  void AssembleResult(evhtp_request_t* request, lbs_res_node_t* res_node, uint64_t time);

  // ip
  std::string ip_;
  // port
  int port_;
  // threads num
  uint32_t threads_num_;
};

}  // namespace mod

#endif  // SERVER_SERVER_H_ 
