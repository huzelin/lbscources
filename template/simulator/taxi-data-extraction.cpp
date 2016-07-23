#include "simulator/taxi-data-extraction.h"

#include <vector>

#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "common/log.h"

namespace mod {

TaxiDataExtraction::TaxiDataExtraction(const std::string& taxi_data_path) :
    taxi_data_path_(taxi_data_path) {
}

TaxiDataExtraction::~TaxiDataExtraction() {
  if (in_.is_open()) {
    in_.close();
  }
}

bool TaxiDataExtraction::HasNext() {
  if (!in_.is_open()) {
    in_.open(taxi_data_path_.c_str());
  }
  if (!in_.is_open()) {
    LOG_ERROR("raw taxi data can not opened: %s", taxi_data_path_.c_str());
    return false;
  }
  std::string line;
  while (std::getline(in_, line)) {
    std::vector<std::string> splits;
    boost::split(splits, line, boost::is_any_of(","));
    if (splits.size() != kTaxiDataCount) {
      LOG_ERROR("raw taxi data column count error: %s", line.c_str());
      continue;
    }
    try {
      taxi_data_.id = splits[kTaxiDataId];
      taxi_data_.lon = boost::lexical_cast<double>(splits[kTaxiDataLon]);
      taxi_data_.lat = boost::lexical_cast<double>(splits[kTaxiDataLat]);
      taxi_data_.speed = boost::lexical_cast<double>(splits[kTaxiDataSpeed]);
      taxi_data_.timestamp = boost::lexical_cast<int64_t>(splits[kTaxiDataTimeStamp]);
      return true;
    } catch (...) {
      LOG_ERROR("raw taxi data error: %s", line.c_str());
      continue;
    }
  }
  return false;
}

}  // namespace mod
