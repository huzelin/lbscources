#include "simulator/controller.h"
#include "common/properties.h"
#include "common/log.h"

using mod::Controller;

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s config\n", argv[0]);
    return 1;
  }
  common::Properties properties;
  if (properties.LoadProperties(argv[1], '=', false) < 0) {
    fprintf(stderr, "LoadProperties %s failed", argv[1]);
    return 1;
  }
  common::SetLogLevel(properties.GetValue("log_level", common::kLogLevelNone));
  Controller& controller = Controller::Instance();
  controller.set_host(properties.GetValue("host", "127.0.0.1"));
  controller.set_port(properties.GetValue("port", 10001));
  controller.set_taxi_data_path(properties.GetValue("taxi_data_path", ""));
  // start run
  controller.Run(properties.GetValue("thread_num", 1));
  return 0;
}
