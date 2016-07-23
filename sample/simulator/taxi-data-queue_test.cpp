#include "simulator/taxi-data-queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

using namespace mod;

int main(int argc, char** argv) {
  TaxiDataQueue taxi_data_queue;
  TaxiData taxi_data;
  taxi_data.id = "10001";
  taxi_data_queue.Enqueue(taxi_data);
  assert(taxi_data_queue.size() == 1);
  taxi_data = taxi_data_queue.Dequeue();
  assert(taxi_data.id == "10001");
  return 0;
}
