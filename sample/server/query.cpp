#include "server/query.h"

#include <vector>

#include "common/log.h"
#include "boost/algorithm/string.hpp"

namespace mod {

bool Query::Parse(struct evkeyvalq& http_query) {
  const char* query  = evhttp_find_header(&http_query, "query");
  const char* range  = evhttp_find_header(&http_query, "range");
  const char* pos    = evhttp_find_header(&http_query, "pos");

  if (query == NULL) {
    LOG_ERROR("query type is NULL");
    return false;
  }
  query_type_.assign(query);
  if (query_type_.compare(kRangeQuery) == 0) {
    if (range == NULL) {
      LOG_ERROR("range argument is NULL");
      return false;
    }
    std::vector<std::string> splits;
    boost::split(splits, range, boost::is_any_of(","));
    if (splits.size() != 4) {
      LOG_ERROR("range value invalid: %s", range);
      return false;
    } else {
      lon1_ = atof(splits[0].c_str());
      lon2_ = atof(splits[1].c_str());
      lat1_ = atof(splits[2].c_str());
      lat2_ = atof(splits[3].c_str());
      return true;
    }
  } else if (query_type_.compare(kNNQuery) == 0) {
    if (pos == NULL) {
      LOG_ERROR("pos argument is NULL");
      return false;
    }
    std::vector<std::string> splits;
    boost::split(splits, pos, boost::is_any_of(","));
    if (splits.size() != 2) {
      LOG_ERROR("pos value invalid: %s", pos);
      return false;
    }
    lon1_ = atof(splits[0].c_str());
    lat1_ = atof(splits[1].c_str());
    return true;
  } else {
    LOG_ERROR("unkown query type: %s", query_type_.c_str());
    return false;
  }
}

}  // namespace mod
