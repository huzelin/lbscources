#ifndef SERVER_GRID_LBS_QUEUE_H_
#define SERVER_GRID_LBS_QUEUE_H_

/***
 *  通用循环链表队列
 *  基本操作：初始化、删除、添加、判断是否为空
 */
typedef struct lbs_queue_s {
  struct lbs_queue_s *prev;
  struct lbs_queue_s *next;
} lbs_queue_t;

#define lbs_queue_init(q) \
    (q)->next = (q); \
    (q)->prev = (q)

#define lbs_queue_remove(q) \
    (q)->prev->next = (q)->next; \
    (q)->next->prev = (q)->prev

#define lbs_queue_insert_head(h, q) \
    (h)->next->prev = (q); \
    (q)->prev       = (h); \
    (q)->next       = (h)->next; \
    (h)->next       = (q)

#define lbs_queue_empty(h) \
    (h)->next == (h)

#endif  // SERVER_GRID_LBS_QUEUE_H_
