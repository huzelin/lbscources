#ifndef MO_MOD_SERVER_GRID_GRID_H_
#define MO_MOD_SERVER_GRID_GRID_H_

#include <pthread.h>

#include "server/grid/defs.h"
#include "server/grid/hash-table.h"

typedef struct Cell {
  // dammy node
  ModNode dammy_node;
  // 锁
  pthread_mutex_t mutex;
} Cell;

typedef struct Grid {
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
  HashTable hash_table;
  // 所有的Cells
  Cell* cell;
} Grid;

// 网格的初始化
void Grid_Init(Grid* grid, double lon1, double lon2, double lat1, double lat2, int row_num, int col_num);

// 更新移动位置
void Grid_Update(Grid* grid, double lon, double lat, uint64_t timestamp, uint32_t id);

// 计算Cell Row
int Grid_CellRow(Grid* grid, double lat);

// 计算Cell Col
int Grid_CellCol(Grid* grid, double lon);

// 计算Cell Id
int Grid_CellId(Grid* grid, int cell_row, int cell_col);

// 计算row和col
void Grid_CellRowCol(Grid* grid, int cell_id, int* cell_row, int* cell_col);

// 获取Cell Id里面的Cell
Cell* Grid_Cell(Grid* grid, int cell_id);

#endif  // MO_MOD_SERVER_GRID_GRID_H_
