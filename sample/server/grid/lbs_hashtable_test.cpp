#include "server/grid/lbs_hashtable.h"

#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {
  lbs_hashtable_t hashtable;
  int ret = lbs_hashtable_init(&hashtable);
  assert(ret == 0);
  printf("ret = %d\n", ret);

  lbs_mov_node_t node;
  node.id = 11;
  lbs_hashtable_set(&hashtable, 11, &node, 12);
  lbs_hashnode_t* f = lbs_hashtable_get(&hashtable, 11);
  assert(f->mov_node == &node);
  f = lbs_hashtable_get(&hashtable, 12);
  assert(f == NULL);
  return 0;
}
