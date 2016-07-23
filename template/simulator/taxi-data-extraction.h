#ifndef SIMULATOR_TAXI_DATA_EXTRACTION_H_
#define SIMULATOR_TAXI_DATA_EXTRACTION_H_

#include <stdint.h>
#include <fstream>

#include "simulator/types.h"

namespace mod {

enum TaxiDataIndex {
  kTaxiDataId = 0,
  kTaxiDataLon,
  kTaxiDataLat,
  kTaxiDataSpeed,
  kTaxiDataTimeStamp,
  kTaxiDataCount,
};

// extract taxi data from file
class TaxiDataExtraction {
 public:
  TaxiDataExtraction(const std::string& taxi_data_path);
  ~TaxiDataExtraction();

  bool HasNext();
  const TaxiData& taxi_data() const { return taxi_data_; }

 private:
  // taxi data path
  std::string taxi_data_path_;
  // taxi data
  TaxiData taxi_data_;
  // ifstream
  std::ifstream in_;
};

}  // namespace mod

#endif // SIMULATOR_TAXI_DATA_EXTRACTION_H_

