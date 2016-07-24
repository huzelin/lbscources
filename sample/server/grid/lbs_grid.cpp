#include "server/grid/lbs_grid.h"

#include "common/log.h"

#include <stdio.h>
#include <stdlib.h>

int lbs_grid_init(lbs_grid_t* lbs_grid,
                  double lon1,
                  double lon2,
                  double lat1,
                  double lat2,
                  int row_num,
                  int col_num) {
  if (NULL == lbs_grid) return -1;
  lbs_grid->lon_min     = lon1;
  lbs_grid->lat_min     = lat1;

  lbs_grid->row_num     = row_num;
  lbs_grid->col_num     = col_num;

  lbs_grid->cell_width  = (lon2 - lon1) / col_num;
  lbs_grid->cell_height = (lat2 - lat1) / row_num;

  lbs_hashtable_init(&(lbs_grid->hash_table));

  int nums = row_num * col_num;
  lbs_grid->cell = (lbs_cell_t*)malloc(sizeof(lbs_cell_t) * nums);
  int i;
  for (i = 0; i < nums; ++i) {
    lbs_cell_t* cell = lbs_grid->cell + i;
    pthread_mutex_init(&(cell->mutex), NULL);
    lbs_queue_init(&(cell->dammy_node.queue));
  }
  return 0;
}

int lbs_grid_destroy(lbs_grid_t* lbs_grid) {
  if (NULL == lbs_grid) return -1;
  return 0;
}

int lbs_grid_update(lbs_grid_t* lbs_grid,
                    double lon,
                    double lat,
                    uint64_t timestamp,
                    uint32_t id) {
  // 计算当前移动对象的cell位置 
  int cell_row = lbs_grid_cell_row(lbs_grid, lat);
  int cell_col = lbs_grid_cell_col(lbs_grid, lon);
  if (cell_row < 0 || cell_col < 0) {
    LOG_ERROR("Invalid CellRow %d CellCol %d (%f %f)\n", cell_row, cell_col, lon, lat);
    return -1;
  }
  int cell_id  = lbs_grid_cell_id(lbs_grid, cell_row, cell_col);
  lbs_hashnode_t* hash_node = lbs_hashtable_get(&(lbs_grid->hash_table), id);
  if (hash_node == NULL) {
    // 新的移动对象
    lbs_mov_node_t* mov_node = (lbs_mov_node_t*)malloc(sizeof(lbs_mov_node_t)); 
    mov_node->lon       = lon;
    mov_node->lat       = lat;
    mov_node->id        = id;
    mov_node->timestamp = timestamp;

    lbs_hashtable_set(&(lbs_grid->hash_table), id, mov_node, cell_id);

    // 插入网格中
    pthread_mutex_lock(&(lbs_grid->cell[cell_id].mutex));
    lbs_queue_insert_head(&(lbs_grid->cell[cell_id].dammy_node.queue), &(mov_node->queue));
    pthread_mutex_unlock(&(lbs_grid->cell[cell_id].mutex));
  } else {
    // 更新移动对象
    int prev_cell_id = hash_node->cell_id;
    pthread_mutex_lock(&(lbs_grid->cell[prev_cell_id].mutex));
    hash_node->mov_node->lon       = lon;
    hash_node->mov_node->lat       = lat;
    hash_node->mov_node->timestamp = timestamp;
    if (prev_cell_id != cell_id) {
      // 跳跃到新的格子里面
      hash_node->cell_id = cell_id;
      lbs_queue_remove(&(hash_node->mov_node->queue));
      pthread_mutex_unlock(&(lbs_grid->cell[prev_cell_id].mutex));

      pthread_mutex_lock(&(lbs_grid->cell[cell_id].mutex));
      lbs_queue_insert_head(&(lbs_grid->cell[cell_id].dammy_node.queue), &(hash_node->mov_node->queue));
      pthread_mutex_unlock(&(lbs_grid->cell[cell_id].mutex));
    } else {
      pthread_mutex_unlock(&(lbs_grid->cell[prev_cell_id].mutex));
    }
  }
  return 0;
}

int lbs_grid_cell_row(lbs_grid_t* lbs_grid, double lat) {
  int num = (lat - lbs_grid->lat_min) / lbs_grid->cell_height;
  if (num < 0)
    return 0;
  else if (num >= lbs_grid->row_num)
    return lbs_grid->row_num;
  return num;
}

int lbs_grid_cell_col(lbs_grid_t* lbs_grid, double lon) {
  int num = (lon - lbs_grid->lon_min) / lbs_grid->cell_width;

  if (num < 0)
   return 0;
  else if (num >= lbs_grid->col_num)
    return lbs_grid->col_num;
  return num;
}

int lbs_grid_cell_id(lbs_grid_t* lbs_grid, int cell_row, int cell_col) {
  return lbs_grid->col_num * cell_row + cell_col;
}

void lbs_grid_cell_row_col(lbs_grid_t* lbs_grid, int cell_id, int* cell_row, int* cell_col) {
  *cell_row = cell_id / lbs_grid->col_num;
  *cell_col = cell_id % lbs_grid->col_num;
}

lbs_cell_t* lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id) {
  if (cell_id < 0 || cell_id >= lbs_grid->row_num * lbs_grid->col_num) {
    return NULL;
  }
  return &(lbs_grid->cell[cell_id]);
}
