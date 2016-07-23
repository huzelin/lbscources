#include "server/grid/index.h"

#include <stdio.h>
#include <stdlib.h>

#include "server/grid/distance.h"
#include "server/grid/grid.h"
#include "server/grid/nn-heap.h"

static Grid g_grid;

#define LON_MIN 116
#define LON_MAX 117
#define LAT_MIN 39
#define LAT_MAX 41
#define ROW_NUM 200
#define COL_NUM 100

// 初始化网格索引
int GridIndex_Init() {
  Grid_Init(&g_grid, LON_MIN, LON_MAX, LAT_MIN, LAT_MAX, ROW_NUM, COL_NUM);
  return 0;
}

// 更新接口[出租车位置更新]
void GridIndex_Update(double lon, double lat, uint64_t timestamp, uint32_t id) {
  Grid_Update(&g_grid, lon, lat, timestamp, id);
}

// 范围查询接口[查询某一范围内的所有出租车信息]
void GridIndex_RangeQuery(double lon1, double lon2, double lat1, double lat2, ResNode* out) {
  int row1 = Grid_CellRow(&g_grid, lat1);
  int row2 = Grid_CellRow(&g_grid, lat2);
  int col1 = Grid_CellCol(&g_grid, lon1);
  int col2 = Grid_CellCol(&g_grid, lon2);

  fprintf(stderr, "row1: %d row2: %d col1: %d col2: %d\n", row1, row2, col1, col2);
  int row, col;
  for (row = row1; row <= row2; ++row) {
    for (col = col1; col <= col2; ++col) {
      int cell_id = Grid_CellId(&g_grid, row, col);
      Cell* cell  = Grid_Cell(&g_grid, cell_id);

      pthread_mutex_lock(&(cell->mutex));
      Queue* head = &(cell->dammy_node.queue);
      Queue* p    = head->next;
      for (; p != head; p = p->next) {
        ModNode* mod_node = (ModNode*)p;
        if (mod_node->lon < lon1 || mod_node->lon > lon2 ||
            mod_node->lat < lat1 || mod_node->lat > lat2) { // 经纬度范围在合理范围之内
          continue;
        }
        ResNode* res      = (ResNode*)malloc(sizeof(ResNode));
        res->lon          = mod_node->lon;
        res->lat          = mod_node->lat;
        res->id           = mod_node->id;
        res->timestamp    = mod_node->timestamp;
        QUEUE_INSERT_HEAD(&(out->queue), &(res->queue));
      }
      pthread_mutex_unlock(&(cell->mutex));
    }
  }
}

//  1 2 3
//  4   5
//  6 7 8
void AddExtension(NNHeap* nn_heap, int cell_id, double lon0, double lat0) {
  int row, col, id;
  double lat, lon, distance;
  ModNode* node = NULL;

  // Position: 1
  id     = cell_id + g_grid.col_num - 1;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) { 
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lat = row * g_grid.cell_height + g_grid.lat_min;
    lon = (col + 1) * g_grid.cell_width + g_grid.lon_min;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }

  // Position: 2
  id         = cell_id + g_grid.col_num;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) { 
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lat = row * g_grid.cell_height + g_grid.lat_min;
    lon = lon0;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }

  // Position: 3
  id         = cell_id + g_grid.col_num + 1;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) {
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lat = row * g_grid.cell_height + g_grid.lat_min;
    lon = col * g_grid.cell_width + g_grid.lon_min;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }

  // Position: 4
  id         = cell_id - 1;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) {
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lat = lat0;
    lon = (col + 1) * g_grid.cell_width + g_grid.lon_min;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }

  // Position: 5
  id         = cell_id + 1;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) {
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lat = lat0;
    lon = col * g_grid.cell_width + g_grid.lon_min;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }

  // Position: 6
  id         = cell_id - g_grid.col_num - 1;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) {
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lat = (row + 1) * g_grid.cell_height + g_grid.lat_min;
    lon = (col + 1) * g_grid.cell_width + g_grid.lon_min;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }

  // Position: 7
  id         = cell_id - g_grid.col_num;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) {
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lon = lon0;
    lat = (row + 1) * g_grid.cell_height + g_grid.lat_min;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }

  // Position: 8
  id         = cell_id - g_grid.col_num + 1;
  if (id >= 0 && id < g_grid.row_num * g_grid.col_num) {
    Grid_CellRowCol(&g_grid, id, &row, &col);
    lat = (row + 1) * g_grid.cell_height + g_grid.lat_min;
    lon = col * g_grid.cell_width + g_grid.lon_min;
    distance = Distance(lon0, lat0, lon, lat);
    node = (ModNode*)(&(g_grid.cell[id].dammy_node));
    NNHeap_Insert(nn_heap, node, id, 1, distance);
  }
}

// NN查询接口[查询离lon,lat最近的出租车]
void GridIndex_NNQuery(double lon, double lat, ResNode* out) {
 int row = Grid_CellRow(&g_grid, lat);
 int col = Grid_CellCol(&g_grid, lon);
 fprintf(stderr, "row = %d col = %d\n", row, col);
 int cell_id = Grid_CellId(&g_grid, row, col);
 // 使用堆进行Best First Search
 NNHeap nn_heap;
 NNHeap_Init(&nn_heap, g_grid.row_num * g_grid.col_num);

 ModNode* node = (ModNode*)(&(g_grid.cell[cell_id].dammy_node));
 double distance = 0;
 NNHeap_Insert(&nn_heap, node, cell_id, 1, distance);

 while (1) {
   HeapNode* heap_node = NNHeap_Top(&nn_heap);
   if (heap_node == NULL) {
     fprintf(stderr, "NNHeap_Top is NULL");
     break;
   }
   HeapNode cur = *heap_node;
   NNHeap_Pop(&nn_heap);

   if (cur.is_grid) {
     Cell* cell  = Grid_Cell(&g_grid, cur.cell_id);
     pthread_mutex_lock(&(cell->mutex));
     // 添加网格内的移动车
     Queue* head = &(cell->dammy_node.queue);
     Queue* p    = head->next;
     for (; p != head; p = p->next) {
        ModNode* mod_node = (ModNode*)p;
        distance = Distance(mod_node->lon, mod_node->lat, lon, lat);
        NNHeap_Insert(&nn_heap, mod_node, cell_id, 0, distance);
     }
     pthread_mutex_unlock(&(cell->mutex));
     // 周边八个网格也需要添加
     AddExtension(&nn_heap, cur.cell_id, lon, lat);
   } else {
     // 添加元素
     ResNode* res      = (ResNode*)malloc(sizeof(ResNode));
     res->lon          = cur.node->lon;
     res->lat          = cur.node->lat;
     res->id           = cur.node->id;
     res->timestamp    = cur.node->timestamp;
     QUEUE_INSERT_HEAD(&(out->queue), &(res->queue));
     break;
   }
 }
 NNHeap_Destroy(&nn_heap);
}
