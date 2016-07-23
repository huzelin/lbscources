#ifndef SERVER_GRID_HASH_TABLE_H_
#define SERVER_GRID_HASH_TABLE_H_

#include <pthread.h>

#include "server/grid/defs.h"

typedef struct HashNode {
  // 链表
  Queue queue;
  // 节点
  ModNode* mod_node;
  // cell id
  int cell_id;
} HashNode;

typedef struct HashTable {
  // 锁
  pthread_mutex_t mutex;
  // 已占用
  int size;
  // 容量
  int capacity;
  // 哈西-链地址
  HashNode* hash_nodes;
} HashTable;

// 初始化
void HashTable_Init(HashTable* hash_table);

// 设置
void HashTable_Set(HashTable* hash_table, uint32_t id, ModNode* mod_node, int cell_id);

// 提取
HashNode* HashTable_Get(HashTable* hash_table, uint32_t id);

// 销毁
void HashTable_Destroy(HashTable* hash_table);

#endif  // SERVER_GRID_HASH_TABLE_H_
