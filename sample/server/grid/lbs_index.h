#ifndef SERVER_GRID_LBS_INDEX_H_
#define SERVER_GRID_LBS_INDEX_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "server/grid/lbs_queue.h"

typedef struct lbs_res_node_s {
  lbs_queue_t queue;   // 返回结果的链表
  double   lon;        // 纬度
  double   lat;        // 经度
  uint32_t id;         // 出租车唯一ID号
  uint64_t timestamp;  // 时间戳
} lbs_res_node_t;

// 初始化网格索引
extern int lbs_grid_index_init();
// 更新接口[出租车位置更新]
extern int lbs_grid_index_update(double lon, double lat, uint64_t timestamp, uint32_t id);
// 范围查询接口[查询某一范围内的所有出租车信息]
extern int lbs_grid_index_range_query(double lon1, double lon2, double lat1, double lat2, lbs_res_node_t* out);
// NN查询接口[查询离lon,lat最近的出租车]
extern int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out);

#ifdef __cplusplus
}
#endif

#endif  // SERVER_GRID_LBS_INDEX_H_
