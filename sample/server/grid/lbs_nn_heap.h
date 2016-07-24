#ifndef SERVER_GRID_LBS_NN_HEAP_H_
#define SERVER_GRID_LBS_NN_HEAP_H_

#include "server/grid/lbs_defs.h"

typedef struct lbs_heapnode_s {
  double distance;   // 距离
  uint8_t is_grid;   // 1:是网格  0:移动对象
  int cell_id;    // cell id
  lbs_mov_node_t* node;
} lbs_heapnode_t;

typedef struct lbs_nnheap_s {
  uint32_t capacity;
  uint32_t size;
  lbs_heapnode_t *heap_nodes;
} lbs_nnheap_t;

/** 初始化 **/
int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap); 
/** 销毁 **/
int lbs_nnheap_destroy(lbs_nnheap_t* lbs_nnheap);
/** 插入 **/
int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node, int cell_id, uint8_t is_grid, double distance); 
/** 获取离distance最小的lbs_heapnode_t ***/
lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t* lbs_nnheap); 
/** 删除堆顶元素 ***/
void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap); 

#endif  // SERVER_GRID_LBS_NN_HEAP_H_
