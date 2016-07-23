#ifndef SERVER_GRID_DEFS_H_
#define SERVER_GRID_DEFS_H_

#include <stdint.h>

#include "server/grid/queue.h"

/***
 *  网格索引内部存储基本单元
 */
typedef struct ModNode {
  Queue    queue;        // 双向队列,在网格中使用
  double   lon;          // 经度
  double   lat;          // 纬度
  uint32_t id;           // 出租车的唯一标识ID号 
  uint64_t timestamp;    // 时间戳
} ModNode;

#endif  // SERVER_GRID_DEFS_H_
