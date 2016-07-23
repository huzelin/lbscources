#include <strings.h>
#include <iostream>

#include "common/properties.h"
#include "common/log.h"
#include "server/server.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " config" << std::endl;
    return 1;
  }
  common::Properties properties;
  if (properties.LoadProperties(argv[1], '=', false) < 0) {
    std::cerr << "LoadProperties " << argv[1] << "failed";
    return 1;
  }
  common::SetLogLevel(properties.GetValue("log_level", common::kLogLevelNone));
  mod::Server server;
  server.set_ip(properties.GetValue("host", "0.0.0.0"));
  server.set_port(properties.GetValue("port", 10001));
  server.set_threads_num(properties.GetValue("threads_num", 2));
  server.Start();
  return 0;
}
