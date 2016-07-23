#ifndef SERVER_QUERY_H_
#define SERVER_QUERY_H_

#include <string>

#include "event2/http.h"
#include "event2/keyvalq_struct.h"
#include "evhtp.h"

namespace mod {

static const std::string kRangeQuery = "range";
static const std::string kNNQuery   = "nn";

class Query {
 public:
  bool Parse(struct evkeyvalq& http_query);

  const std::string& query_type() const {
    return query_type_;
  }
  void GetRange(double& lon1, double& lon2, double& lat1, double& lat2) {
    lon1 = lon1_;
    lon2 = lon2_;
    lat1 = lat1_;
    lat2 = lat2_;
  }
  void GetPos(double& lon, double& lat) {
    lon = lon1_;
    lat = lat1_;
  }

 private:
  std::string query_type_;
  double lon1_;
  double lon2_;
  double lat1_;
  double lat2_;
};

}  // namespace mod

#endif  // SERVER_QUERY_H_
