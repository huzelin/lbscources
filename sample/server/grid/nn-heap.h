#ifndef SERVER_GRID_HEAP_H_
#define SERVER_GRID_HEAP_H_

#include "server/grid/defs.h"

typedef struct HeapNode {
  double distance;   // 距离
  uint8_t is_grid;   // 1:是网格  0:移动对象
  int    cell_id;    // cell id
  ModNode* node;
} HeapNode;

typedef struct Bitmap {
  uint8_t *bits;
  uint32_t bits_num;
} Bitmap;

typedef struct NNHeap {
  uint32_t capacity;
  uint32_t size;
  Bitmap bitmap;
  HeapNode *heap_nodes;
} NNHeap;

/** 初始化 **/
void NNHeap_Init(NNHeap* heap, uint32_t grid_num); 
/** 销毁 **/
void NNHeap_Destroy(NNHeap* heap);
/** 插入 **/
void NNHeap_Insert(NNHeap* heap, ModNode* node, int cell_id, uint8_t is_grid, double distance); 
/** 获取离distance最小的HeapNode ***/
HeapNode* NNHeap_Top(NNHeap* heap); 
/** 删除堆顶元素 ***/
void NNHeap_Pop(NNHeap* heap); 

#endif  // SERVER_GRID_HEAP_H_
