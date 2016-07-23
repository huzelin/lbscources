#include "server/grid/index.h"

#include <stdio.h>
#include <stdlib.h>

// 初始化网格索引
int GridIndex_Init() {

  return 0;
}

// 更新接口[出租车位置更新]
void GridIndex_Update(double lon, double lat, uint64_t timestamp, uint32_t id) {

}

// 范围查询接口[查询某一范围内的所有出租车信息]
void GridIndex_RangeQuery(double lon1, double lon2, double lat1, double lat2, ResNode* out) {
 
}

// NN查询接口[查询离lon,lat最近的出租车]
void GridIndex_NNQuery(double lon, double lat, ResNode* out) {

}
