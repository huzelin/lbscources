#include "server/grid/nn-heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEAP_INIT_CAPACITY 10
static uint8_t BITMAP_CODE[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

void NNHeap_Init(NNHeap* heap, uint32_t grid_num) {
  heap->capacity   = HEAP_INIT_CAPACITY;
  heap->size       = 0;
  heap->heap_nodes = (HeapNode*)malloc(heap->capacity * sizeof(HeapNode));

  heap->bitmap.bits_num = grid_num;
  int32_t cnt           = grid_num / 8 + (grid_num % 8 == 0 ? 0 : 1);
  heap->bitmap.bits     = (uint8_t*)malloc(cnt);
  memset(heap->bitmap.bits, 0, cnt);
}

void NNHeap_Destroy(NNHeap* heap) {
  free(heap->heap_nodes);
  free(heap->bitmap.bits);
}

static void NNHeap_AdjustFromBottom(NNHeap* heap) {
  uint32_t idx = heap->size;
  while (idx > 1) {
    uint32_t parent = idx / 2;
    int swap = 0;
    if (heap->heap_nodes[parent - 1].distance > heap->heap_nodes[idx - 1].distance) {
      swap = 1;
    }
    if (swap) {
      HeapNode tmp = heap->heap_nodes[parent - 1];
      heap->heap_nodes[parent - 1] = heap->heap_nodes[idx - 1];
      heap->heap_nodes[idx - 1] = tmp;
      idx = parent;
    } else {
      break;
    }
  }
}

void NNHeap_Insert(NNHeap* heap, ModNode* node, int cell_id, uint8_t is_grid, double distance) {
  if (heap->size == heap->capacity) {  // 堆栈满，重新分配空间
    heap->capacity   = heap->capacity * 3 / 2;
    heap->heap_nodes = (HeapNode*)realloc(heap->heap_nodes, heap->capacity * sizeof(HeapNode));
  }
  if (is_grid) { // 如果是网格
    int bitmap_idx = cell_id / 8;
    int offset     = cell_id % 8;
    if ((heap->bitmap.bits[bitmap_idx] & BITMAP_CODE[offset]) != 0) {
      return;
    } else {
      heap->bitmap.bits[bitmap_idx] |= BITMAP_CODE[offset];
    }
  }
  uint32_t idx = heap->size;
  heap->heap_nodes[idx].node = node;
  heap->heap_nodes[idx].cell_id = cell_id;
  heap->heap_nodes[idx].is_grid = is_grid;
  heap->heap_nodes[idx].distance = distance;
  heap->size++;
  NNHeap_AdjustFromBottom(heap);
}

HeapNode* NNHeap_Top(NNHeap* heap) {
  if (heap->size > 0) {
    return heap->heap_nodes;
  }
  return NULL;
}

void NNHeap_AdjustFromTop(NNHeap* heap) {
  uint32_t idx = 1;
  while (2 * idx <= heap->size) {
    uint32_t min = 2 * idx;
    uint32_t right = min + 1;
    if (right <= heap->size) {
      if (heap->heap_nodes[right - 1].distance < heap->heap_nodes[min - 1].distance) {
        min = right;
      }
    }
    if (heap->heap_nodes[idx - 1].distance > heap->heap_nodes[min].distance) {
      HeapNode tmp = heap->heap_nodes[idx - 1];
      heap->heap_nodes[idx - 1] = heap->heap_nodes[min - 1];
      heap->heap_nodes[min - 1] = tmp;
      idx = min;
    } else {
      break;
    }
  }
}

void NNHeap_Pop(NNHeap* heap) {
  heap->size--;
  if (heap->size <= 0)
    return;
  heap->heap_nodes[0] = heap->heap_nodes[heap->size];
  NNHeap_AdjustFromTop(heap); 
}
