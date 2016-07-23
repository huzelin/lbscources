#ifndef SERVER_GRID_INDEX_H_
#define SERVER_GRID_INDEX_H_

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "server/grid/queue.h"

typedef struct ResNode {
  Queue    queue;      // 返回结果的链表
  double   lon;        // 纬度
  double   lat;        // 经度
  uint32_t id;         // 出租车唯一ID号
  uint64_t timestamp;  // 时间戳
} ResNode;

// 初始化网格索引
extern int GridIndex_Init();

// 更新接口[出租车位置更新]
extern void GridIndex_Update(double lon, double lat, uint64_t timestamp, uint32_t id);

// 范围查询接口[查询某一范围内的所有出租车信息]
extern void GridIndex_RangeQuery(double lon1, double lon2, double lat1, double lat2, ResNode* out);

// NN查询接口[查询离lon,lat最近的出租车]
extern void GridIndex_NNQuery(double lon, double lat, ResNode* out);

#ifdef __cplusplus
}
#endif

#endif  // SERVER_GRID_INDEX_H_
