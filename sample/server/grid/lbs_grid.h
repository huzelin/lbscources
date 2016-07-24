#ifndef SERVER_GRID_LBS_GRID_H_
#define SERVER_GRID_LBS_GRID_H_

#include <pthread.h>

#include "server/grid/lbs_defs.h"
#include "server/grid/lbs_hashtable.h"

typedef struct lbs_cell_s {
  // dammy node
  lbs_mov_node_t dammy_node;
  // 锁
  pthread_mutex_t mutex;
} lbs_cell_t;

typedef struct lbs_grid_s {
  // row num of grid
  int row_num;
  // col num of grid
  int col_num;
  // cell width
  double cell_width;
  // cell height
  double cell_height;
  // grid lon minimum value
  double lon_min;
  // grid lat minimum value
  double lat_min;
  // 哈西表
  lbs_hashtable_t hash_table;
  // 所有的Cells
  lbs_cell_t* cell;
} lbs_grid_t;

// 网格的初始化
int lbs_grid_init(lbs_grid_t* lbs_grid, double lon1, double lon2, double lat1, double lat2, int row_num, int col_num);
// 网格的删除
int lbs_grid_destroy(lbs_grid_t* lbs_grid);
// 更新移动位置
int lbs_grid_update(lbs_grid_t* lbs_grid, double lon, double lat, uint64_t timestamp, uint32_t id);
// 计算Cell Row
int lbs_grid_cell_row(lbs_grid_t* lbs_grid, double lat);
// 计算Cell Col
int lbs_grid_cell_col(lbs_grid_t* lbs_grid, double lon);
// 计算Cell Id
int lbs_grid_cell_id(lbs_grid_t* lbs_grid, int cell_row, int cell_col);
// 计算row和col
void lbs_grid_cell_row_col(lbs_grid_t* lbs_grid, int cell_id, int* cell_row, int* cell_col);
// 获取Cell Id里面的Cell
lbs_cell_t* lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id);

#endif  // SERVER_GRID_LBS_GRID_H_
