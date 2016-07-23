#ifndef SIMULATOR_TYPE_H_
#define SIMULATOR_TYPE_H_

#include <stdint.h>
#include <string>

namespace mod {

typedef struct TaxiData {
  std::string id;
  double lon;
  double lat;
  double speed;
  int64_t timestamp;
} TaxiData;

}  // namespace mod

#endif  // SIMULATOR_TYPE_H_
