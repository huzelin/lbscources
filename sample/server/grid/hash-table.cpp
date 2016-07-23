#include "server/grid/hash-table.h"

#include <stdio.h>
#include <stdlib.h>

#define kDefaultHashCapacity 200000

void HashTable_Init(HashTable* hash_table) {
  hash_table->size       = 0;
  hash_table->capacity   = kDefaultHashCapacity;
  hash_table->hash_nodes = (HashNode*)malloc(sizeof(HashNode) * hash_table->capacity);
 
  int i = 0; 
  for (; i < hash_table->capacity; ++i) {
    HashNode* node = hash_table->hash_nodes + i;
    QUEUE_INIT(&(node->queue));
  }
  pthread_mutex_init(&(hash_table->mutex), NULL);
}

void HashTable_Set(HashTable* hash_table, uint32_t id, ModNode* mod_node, int cell_id) {
  uint32_t index     = id % hash_table->capacity;
  HashNode* head     = hash_table->hash_nodes + index;

  HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
  new_node->mod_node = mod_node;
  new_node->cell_id  = cell_id;

  pthread_mutex_lock(&(hash_table->mutex));
  QUEUE_INSERT_HEAD(&(head->queue), &(new_node->queue)); 
  pthread_mutex_unlock(&(hash_table->mutex));
}

HashNode* HashTable_Get(HashTable* hash_table, uint32_t id) {
  uint32_t index     = id % hash_table->capacity;
  HashNode* head     = hash_table->hash_nodes + index;

  pthread_mutex_lock(&(hash_table->mutex));
  HashNode* next     = (HashNode*)(head->queue.next);
  HashNode*  ret     = NULL;
  for (; next != head; next = (HashNode*)(next->queue.next)) {
    if (id == next->mod_node->id) {
      ret = next;
      break;
    }
  }
  pthread_mutex_unlock(&(hash_table->mutex));
  return ret;
}

void HashTable_Destroy(HashTable* hash_table) {
  free(hash_table->hash_nodes);
}
