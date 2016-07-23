#include "server/grid/grid.h"

#include <stdio.h>
#include <stdlib.h>

void Grid_Init(Grid* grid, double lon1, double lon2, double lat1, double lat2, int row_num, int col_num) {
  grid->lon_min     = lon1;
  grid->lat_min     = lat1;

  grid->row_num     = row_num;
  grid->col_num     = col_num;

  grid->cell_width  = (lon2 - lon1) / col_num;
  grid->cell_height = (lat2 - lat1) / row_num;

  HashTable_Init(&(grid->hash_table));

  int nums = row_num * col_num;
  grid->cell = (Cell*)malloc(sizeof(Cell) * nums);
  int i;
  for (i = 0; i < nums; ++i) {
    Cell* cell = grid->cell + i;
    pthread_mutex_init(&(cell->mutex), NULL);
    QUEUE_INIT(&(cell->dammy_node.queue));
  }
}

void Grid_Update(Grid* grid, double lon, double lat, uint64_t timestamp, uint32_t id) {
  HashNode* hash_node = HashTable_Get(&(grid->hash_table), id);
  // 计算当前移动对象的cell位置 
  int cell_row = Grid_CellRow(grid, lat);
  int cell_col = Grid_CellCol(grid, lon);
  if (cell_row < 0 || cell_col < 0) {
    fprintf(stderr, "Invalid CellRow %d CellCol %d (%f, %f)\n", cell_row, cell_col, lon, lat);
    return;
  }
  int cell_id  = Grid_CellId(grid, cell_row, cell_col);
  
  if (hash_node == NULL) {
    // 新的移动对象
    ModNode* mod_node = (ModNode*)malloc(sizeof(ModNode)); 
    mod_node->lon       = lon;
    mod_node->lat       = lat;
    mod_node->id        = id;
    mod_node->timestamp = timestamp;

    HashTable_Set(&(grid->hash_table), id, mod_node, cell_id);

    // 插入网格中
    pthread_mutex_lock(&(grid->cell[cell_id].mutex));
    QUEUE_INSERT_HEAD(&(grid->cell[cell_id].dammy_node.queue),
                      &(mod_node->queue));
    pthread_mutex_unlock(&(grid->cell[cell_id].mutex));
  } else {
    // 更新移动对象
    int prev_cell_id = hash_node->cell_id;
    pthread_mutex_lock(&(grid->cell[prev_cell_id].mutex));
    hash_node->mod_node->lon       = lon;
    hash_node->mod_node->lat       = lat;
    hash_node->mod_node->timestamp = timestamp;
    if (prev_cell_id != cell_id) {
      // 跳跃到新的格子里面
      hash_node->cell_id = cell_id;
      QUEUE_REMOVE(&(hash_node->mod_node->queue));
      pthread_mutex_unlock(&(grid->cell[prev_cell_id].mutex));

      pthread_mutex_lock(&(grid->cell[cell_id].mutex));
      QUEUE_INSERT_HEAD(&(grid->cell[cell_id].dammy_node.queue), &(hash_node->mod_node->queue));
      pthread_mutex_unlock(&(grid->cell[cell_id].mutex));
    } else {
      pthread_mutex_unlock(&(grid->cell[prev_cell_id].mutex));
    }
  }
}

int Grid_CellRow(Grid* grid, double lat) {
  int num = (lat - grid->lat_min) / grid->cell_height;

  if (num < 0)
    num = 0;
  else if (num >= grid->row_num)
    num = grid->row_num -1;

  return num;
}

int Grid_CellCol(Grid* grid, double lon) {
  int num = (lon - grid->lon_min) / grid->cell_width;

  if (num < 0)
    num = 0;
  else if (num >= grid->col_num)
    num = grid->col_num - 1;

  return num;
}

int Grid_CellId(Grid* grid, int cell_row, int cell_col) {
  return grid->col_num * cell_row + cell_col;
}

void Grid_CellRowCol(Grid* grid, int cell_id, int* cell_row, int* cell_col) {
  *cell_row = cell_id / grid->col_num;
  *cell_col = cell_id % grid->col_num;
}

Cell* Grid_Cell(Grid* grid, int cell_id) {
  if (cell_id < 0 || cell_id >= grid->row_num * grid->col_num) {
    return NULL;
  }
  return &(grid->cell[cell_id]);
}
