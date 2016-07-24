#include "server/grid/lbs_nn_heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LBS_NNHEAP_INIT_CAPACITY 10

int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap) {
  if (lbs_nnheap == NULL) return -1;
  lbs_nnheap->capacity   = LBS_NNHEAP_INIT_CAPACITY;
  lbs_nnheap->size       = 0;
  lbs_nnheap->heap_nodes = (lbs_heapnode_t*)malloc(lbs_nnheap->capacity * sizeof(lbs_heapnode_t));
  return 0;
}

int lbs_nnheap_destroy(lbs_nnheap_t* lbs_nnheap) {
  if (lbs_nnheap == NULL) return -1;
  free(lbs_nnheap->heap_nodes);
  return 0;
}

static void lbs_nnheap_adjust_from_bottom(lbs_nnheap_t* lbs_nnheap) {
  uint32_t idx = lbs_nnheap->size;
  while (idx > 1) {
    uint32_t parent = idx / 2;
    int swap = 0;
    if (lbs_nnheap->heap_nodes[parent - 1].distance > lbs_nnheap->heap_nodes[idx - 1].distance) {
      swap = 1;
    }
    if (swap) {
      lbs_heapnode_t tmp = lbs_nnheap->heap_nodes[parent - 1];
      lbs_nnheap->heap_nodes[parent - 1] = lbs_nnheap->heap_nodes[idx - 1];
      lbs_nnheap->heap_nodes[idx - 1] = tmp;
      idx = parent;
    } else {
      break;
    }
  }
}

int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap,
                       lbs_mov_node_t* lbs_mov_node,
                       int cell_id,
                       uint8_t is_grid,
                       double distance) {
  if (lbs_nnheap == NULL || lbs_mov_node == NULL) return -1;
  if (lbs_nnheap->size == lbs_nnheap->capacity) {  // 堆栈满，重新分配空间
    lbs_nnheap->capacity   = lbs_nnheap->capacity * 3 / 2;
    lbs_nnheap->heap_nodes = (lbs_heapnode_t*)realloc(lbs_nnheap->heap_nodes,
                                                      lbs_nnheap->capacity * sizeof(lbs_heapnode_t));
  }
  uint32_t idx = lbs_nnheap->size;
  lbs_nnheap->heap_nodes[idx].node = lbs_mov_node;
  lbs_nnheap->heap_nodes[idx].cell_id = cell_id;
  lbs_nnheap->heap_nodes[idx].is_grid = is_grid;
  lbs_nnheap->heap_nodes[idx].distance = distance;
  lbs_nnheap->size++;
  lbs_nnheap_adjust_from_bottom(lbs_nnheap);
  return 0;
}

lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t* lbs_nnheap) {
  if (lbs_nnheap->size > 0) {
    return lbs_nnheap->heap_nodes;
  }
  return NULL;
}

static void lbs_nnheap_adjust_from_top(lbs_nnheap_t* lbs_nnheap) {
  uint32_t idx = 1;
  while (2 * idx <= lbs_nnheap->size) {
    uint32_t min = 2 * idx;
    uint32_t right = min + 1;
    if (right <= lbs_nnheap->size) {
      if (lbs_nnheap->heap_nodes[right - 1].distance < lbs_nnheap->heap_nodes[min - 1].distance) {
        min = right;
      }
    }
    if (lbs_nnheap->heap_nodes[idx - 1].distance > lbs_nnheap->heap_nodes[min].distance) {
      lbs_heapnode_t tmp = lbs_nnheap->heap_nodes[idx - 1];
      lbs_nnheap->heap_nodes[idx - 1] = lbs_nnheap->heap_nodes[min - 1];
      lbs_nnheap->heap_nodes[min - 1] = tmp;
      idx = min;
    } else {
      break;
    }
  }
}

void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap) {
  lbs_nnheap->size--;
  if (lbs_nnheap->size <= 0)
    return;
  lbs_nnheap->heap_nodes[0] = lbs_nnheap->heap_nodes[lbs_nnheap->size];
  lbs_nnheap_adjust_from_top(lbs_nnheap); 
}
