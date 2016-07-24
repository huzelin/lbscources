#ifndef SERVER_GRID_LBS_BITMAP_H_
#define SERVER_GRID_LBS_BITMAP_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct lbs_bitmap_s {
  uint8_t *bits;
  uint32_t bits_num;
} lbs_bitmap_t;

/** 初始化Bitmap **/
int lbs_bitmap_init(lbs_bitmap_t* lbs_bitmap, uint32_t bits_num);
/** 销毁 **/
int lbs_bitmap_destroy(lbs_bitmap_t* lbs_bitmap);
/** 设置Bit**/
int lbs_bitmap_setbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos);
/** 取消设置Bit **/
int lbs_bitmap_unsetbit(lbs_bitmap_t* lbs_bitmap, uint32_t pos);
/** 判定是否设置Bit **/
int lbs_bitmap_isset(lbs_bitmap_t* lbs_bitmap, uint32_t pos);

#endif  // SERVER_GRID_BITMAP_H_
