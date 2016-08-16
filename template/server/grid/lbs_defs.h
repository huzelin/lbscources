#ifndef SERVER_GRID_DEFS_H_
#define SERVER_GRID_DEFS_H_

#include <stdint.h>

#include "server/grid/lbs_queue.h"

/***
 *  网格索引内部存储基本单元
 */
typedef struct lbs_mov_node_s {
  lbs_queue_t queue;     // 双向队列,在网格中使用
  double   lon;          // 经度
  double   lat;          // 纬度
  uint32_t id;           // 出租车的唯一标识ID号 
  uint64_t timestamp;    // 时间戳
} lbs_mov_node_t;

typedef struct lbs_cell_s {
  lbs_mov_node_t head;
  pthread_mutex_t mutex;
} lbs_cell_t;

typedef struct lbs_grid_s {
  int col;
  int row;
  float lon_min;
  float lat_min;
  float cell_width;
  float cell_height;
  lbs_cell_t* cells;
  lbs_hashtable_t hashtable;
} lbs_grid_t;

extern int lbs_grid_init(lbs_grid_t* grid, float lon1, float lon2, float lat1, float lat2, int row, int col);
extern int lbs_grid_update(lbs_grid_t* grid, float lon, float lat, uint32_t id);

extern int lbs_grid_cell_row(lbs_grid_t* grid, float lat);
extern int lbs_grid_cell_col(lbs_grid_t* grid, float lon);
extern int lbs_grid_cell_id(lbs_grid_t* grid, int cell_row, int cell_col);





#endif  // SERVER_GRID_DEFS_H_
