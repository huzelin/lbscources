#include "server/grid/lbs_index.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/log.h"

#include "server/grid/lbs_distance.h"

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

// 初始化网格索引
int lbs_grid_index_init() {
  // TODO: by student
  return 0;
}

// 更新接口[出租车位置更新]
int lbs_grid_index_update(double lon,
                           double lat,
                           uint64_t timestamp,
                           uint32_t id) {
  // TODO: by student
  return 0;
}

// 范围查询接口[查询某一范围内的所有出租车信息]
int lbs_grid_index_range_query(double lon1,
                               double lon2,
                               double lat1,
                               double lat2,
                               lbs_res_node_t* out) {
  // TODO: by student
  return 0;
}

// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
  // TODO: by student
  return 0;
}
