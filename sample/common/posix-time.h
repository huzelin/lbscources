#ifndef COMMON_POSIX_TIME_H_
#define COMMON_POSIX_TIME_H_

#include <stdint.h>
#include <time.h>
#include <sys/time.h>

namespace common {

inline void RelativeTimeInMillisecondsToAbsTimeSpec(
    int64_t relative_time_in_ms,
    timespec* ts) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t usec = tv.tv_usec + relative_time_in_ms * 1000LL;
  ts->tv_sec = tv.tv_sec + usec / 1000000;
  ts->tv_nsec = (usec % 1000000) * 1000;
}

inline uint64_t GetTime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec*1000000 + tv.tv_usec;
}

inline void Sleep(uint32_t ms) {
  struct timespec slptm;
  slptm.tv_sec = ms / 1000;
  slptm.tv_nsec = (ms % 1000) * 1000 * 1000;
  nanosleep(&slptm, NULL);
}

} //namespace

#endif // COMMON_POSIX_TIME_H_
