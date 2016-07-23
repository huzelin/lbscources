#ifndef SIMULATOR_TAXI_DATA_QUEUE_H_
#define SIMULATOR_TAXI_DATA_QUEUE_H_

#include <queue>
#include <pthread.h>

#include "simulator/types.h"

namespace mod {

class TaxiDataQueue {
 public:
  TaxiDataQueue();
  ~TaxiDataQueue();

  void Enqueue(const TaxiData& taxi_data);
  TaxiData Dequeue();

  void set_max_size(size_t max_size) { max_size_ = max_size; }
  size_t max_size() const { return max_size_; }
  size_t size() const { return taxi_datas_.size(); }

 private:
  // maximum data size
  size_t max_size_;
  // all taxi datas from file
  std::queue<TaxiData> taxi_datas_;
  // mutex
  pthread_mutex_t mutex_;
  // condition for full
  pthread_cond_t  cond_full_;
  // condition for empty
  pthread_cond_t  cond_empty_;
};

}  // namespace mod

#endif  // SIMULATOR_TAXI_DATA_QUEUE_H_
