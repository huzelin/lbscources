#include "server/grid/lbs_index.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/log.h"

#include "server/grid/lbs_distance.h"
#include "server/grid/lbs_grid.h"
#include "server/grid/lbs_nn_heap.h"
#include "server/grid/lbs_bitmap.h"

static lbs_grid_t lbs_grid;

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

// 初始化网格索引
int lbs_grid_index_init() {
  return lbs_grid_init(&lbs_grid,
                       LBS_LON_MIN,
                       LBS_LON_MAX,
                       LBS_LAT_MIN,
                       LBS_LAT_MAX,
                       LBS_ROW_NUM,
                       LBS_COL_NUM);
}

// 更新接口[出租车位置更新]
int lbs_grid_index_update(double lon,
                           double lat,
                           uint64_t timestamp,
                           uint32_t id) {
  return lbs_grid_update(&lbs_grid, lon, lat, timestamp, id);
}

// 范围查询接口[查询某一范围内的所有出租车信息]
int lbs_grid_index_range_query(double lon1,
                               double lon2,
                               double lat1,
                               double lat2,
                               lbs_res_node_t* out) {
  if (NULL == out) return -1;
  int row1 = lbs_grid_cell_row(&lbs_grid, lat1);
  int row2 = lbs_grid_cell_row(&lbs_grid, lat2);
  int col1 = lbs_grid_cell_col(&lbs_grid, lon1);
  int col2 = lbs_grid_cell_col(&lbs_grid, lon2);

  LOG_DEBUG("row1: %d row2: %d col1: %d col2: %d\n", row1, row2, col1, col2);
  int row, col;
  for (row = row1; row <= row2; ++row) {
    for (col = col1; col <= col2; ++col) {
      int cell_id = lbs_grid_cell_id(&lbs_grid, row, col);
      lbs_cell_t* cell  = lbs_grid_cell(&lbs_grid, cell_id);

      pthread_mutex_lock(&(cell->mutex));
      lbs_queue_t* head = &(cell->dammy_node.queue);
      lbs_queue_t* p    = head->next;
      for (; p != head; p = p->next) {
        lbs_mov_node_t* mov_node = (lbs_mov_node_t*)p;
        if (mov_node->lon < lon1 || mov_node->lon > lon2 ||
            mov_node->lat < lat1 || mov_node->lat > lat2) { // 经纬度范围在合理范围之内
          continue;
        }
        lbs_res_node_t* res = (lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));
        res->lon          = mov_node->lon;
        res->lat          = mov_node->lat;
        res->id           = mov_node->id;
        res->timestamp    = mov_node->timestamp;
        lbs_queue_insert_head(&(out->queue), &(res->queue));
      }
      pthread_mutex_unlock(&(cell->mutex));
    }
  }
  return 0;
}

//  1 2 3
//  4   5
//  6 7 8
static void lbs_add_extension(lbs_nnheap_t* lbs_nnheap, lbs_bitmap_t* lbs_bitmap, int cell_id, double lon0, double lat0) {
  int row, col, id;
  double lat, lon, distance;
  lbs_mov_node_t* node = NULL;

  // Position: 1
  id     = cell_id + lbs_grid.col_num - 1;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) { 
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lat = row * lbs_grid.cell_height + lbs_grid.lat_min;
    lon = (col + 1) * lbs_grid.cell_width + lbs_grid.lon_min;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }

  // Position: 2
  id         = cell_id + lbs_grid.col_num;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) { 
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lat = row * lbs_grid.cell_height + lbs_grid.lat_min;
    lon = lon0;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }

  // Position: 3
  id         = cell_id + lbs_grid.col_num + 1;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) {
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lat = row * lbs_grid.cell_height + lbs_grid.lat_min;
    lon = col * lbs_grid.cell_width + lbs_grid.lon_min;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }

  // Position: 4
  id         = cell_id - 1;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) {
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lat = lat0;
    lon = (col + 1) * lbs_grid.cell_width + lbs_grid.lon_min;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }

  // Position: 5
  id         = cell_id + 1;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) {
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lat = lat0;
    lon = col * lbs_grid.cell_width + lbs_grid.lon_min;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }

  // Position: 6
  id         = cell_id - lbs_grid.col_num - 1;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) {
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lat = (row + 1) * lbs_grid.cell_height + lbs_grid.lat_min;
    lon = (col + 1) * lbs_grid.cell_width + lbs_grid.lon_min;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }

  // Position: 7
  id         = cell_id - lbs_grid.col_num;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) {
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lon = lon0;
    lat = (row + 1) * lbs_grid.cell_height + lbs_grid.lat_min;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }

  // Position: 8
  id         = cell_id - lbs_grid.col_num + 1;
  if (id >= 0 && id < lbs_grid.row_num * lbs_grid.col_num && lbs_bitmap_isset(lbs_bitmap, id) == 0) {
    lbs_grid_cell_row_col(&lbs_grid, id, &row, &col);
    lat = (row + 1) * lbs_grid.cell_height + lbs_grid.lat_min;
    lon = col * lbs_grid.cell_width + lbs_grid.lon_min;
    distance = lbs_distance(lon0, lat0, lon, lat);
    node = (lbs_mov_node_t*)(&(lbs_grid.cell[id].dammy_node));
    lbs_nnheap_insert(lbs_nnheap, node, id, 1, distance);
  }
}

// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
 int row = lbs_grid_cell_row(&lbs_grid, lat);
 int col = lbs_grid_cell_col(&lbs_grid, lon);
 int cell_id = lbs_grid_cell_id(&lbs_grid, row, col);

 // 使用堆进行Best First Search
 lbs_nnheap_t lbs_nnheap;
 lbs_nnheap_init(&lbs_nnheap);
 lbs_bitmap_t lbs_bitmap;
 lbs_bitmap_init(&lbs_bitmap, lbs_grid.row_num * lbs_grid.col_num);

 lbs_mov_node_t* node = (lbs_mov_node_t*)(&(lbs_grid.cell[cell_id].dammy_node));
 double distance = 0;
 lbs_nnheap_insert(&lbs_nnheap, node, cell_id, 1, distance);

 while (1) {
   lbs_heapnode_t* heap_node = lbs_nnheap_top(&lbs_nnheap);
   if (heap_node == NULL) {
     fprintf(stderr, "NNHeap_Top is NULL");
     break;
   }
   lbs_heapnode_t cur = *heap_node;
   lbs_nnheap_pop(&lbs_nnheap);

   if (cur.is_grid) {
     lbs_cell_t* cell  = lbs_grid_cell(&lbs_grid, cur.cell_id);
     if (!cell) continue;
     if (lbs_bitmap_isset(&lbs_bitmap, cur.cell_id) == 0) {
       pthread_mutex_lock(&(cell->mutex));
       // 添加网格内的移动车
       lbs_queue_t* head = &(cell->dammy_node.queue);
       lbs_queue_t* p    = head->next;
       for (; p != head; p = p->next) {
         lbs_mov_node_t* mod_node = (lbs_mov_node_t*)p;
         distance = lbs_distance(mod_node->lon, mod_node->lat, lon, lat);
         lbs_nnheap_insert(&lbs_nnheap, mod_node, cell_id, 0, distance);
       }
       pthread_mutex_unlock(&(cell->mutex));
       lbs_bitmap_setbit(&lbs_bitmap, cur.cell_id);
     }
     // 周边八个网格也需要添加
     lbs_add_extension(&lbs_nnheap, &lbs_bitmap, cur.cell_id, lon, lat);
   } else {
     // 添加元素
     lbs_res_node_t* res = (lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));
     res->lon          = cur.node->lon;
     res->lat          = cur.node->lat;
     res->id           = cur.node->id;
     res->timestamp    = cur.node->timestamp;
     lbs_queue_insert_head(&(out->queue), &(res->queue));
     break;
   }
 }
 return lbs_nnheap_destroy(&lbs_nnheap);
}
