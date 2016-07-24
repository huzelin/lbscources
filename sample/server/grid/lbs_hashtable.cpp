#include "server/grid/lbs_hashtable.h"

#include <stdio.h>
#include <stdlib.h>

#define LBS_DEFAULT_HASH_CAPACITY 1000000

// 初始化hashtable
int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable) {
  if (lbs_hashtable == NULL) return -1;
  lbs_hashtable->size       = 0;
  lbs_hashtable->capacity   = LBS_DEFAULT_HASH_CAPACITY;
  lbs_hashtable->hash_nodes = (lbs_hashnode_t*)malloc(sizeof(lbs_hashnode_t) * lbs_hashtable->capacity);
 
  int i = 0; 
  for (; i < lbs_hashtable->capacity; ++i) {
    lbs_hashnode_t* node = lbs_hashtable->hash_nodes + i;
    lbs_queue_init(&(node->queue));
  }
  pthread_mutex_init(&(lbs_hashtable->mutex), NULL);
  return 0;
}

int lbs_hashtable_destroy(lbs_hashtable_t* lbs_hashtable) {
  if (lbs_hashtable == NULL) return -1;
  free(lbs_hashtable->hash_nodes);
  return 0;
}

int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id) {
  if (lbs_hashtable == NULL) return -1;
  uint32_t index       = id % lbs_hashtable->capacity;
  lbs_hashnode_t* head = lbs_hashtable->hash_nodes + index;

  lbs_hashnode_t* new_node = (lbs_hashnode_t*)malloc(sizeof(lbs_hashnode_t));
  new_node->mov_node = lbs_mov_node;
  new_node->cell_id  = cell_id;

  pthread_mutex_lock(&(lbs_hashtable->mutex));
  lbs_queue_insert_head(&(head->queue), &(new_node->queue)); 
  pthread_mutex_unlock(&(lbs_hashtable->mutex));
  return 0;
}

lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hashtable, uint32_t id) {
  uint32_t index       = id % lbs_hashtable->capacity;
  lbs_hashnode_t* head = lbs_hashtable->hash_nodes + index;

  pthread_mutex_lock(&(lbs_hashtable->mutex));
  lbs_hashnode_t* next = (lbs_hashnode_t*)(head->queue.next);
  lbs_hashnode_t* ret  = NULL;
  for (; next != head; next = (lbs_hashnode_t*)(next->queue.next)) {
    if (id == next->mov_node->id) {
      ret = next;
      break;
    }
  }
  pthread_mutex_unlock(&(lbs_hashtable->mutex));
  return ret;
}

