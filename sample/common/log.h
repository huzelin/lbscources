#ifndef COMMON_LOG_H_
#define COMMON_LOG_H_

#include <stdio.h>

namespace common {
enum {
  kLogLevelDebug = 1,
  kLogLevelError,
  kLogLevelNone,
};

extern void SetLogLevel(int log_level);
extern int GetLogLevel();
}  // namespace common

#ifndef LOG_DEBUG
#define LOG_DEBUG(fmt, ...) do { \
  if (common::GetLogLevel() <= common::kLogLevelDebug) { \
    fprintf(stderr, "[DEBUG] [%s:%d:%s()]" fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
  }\
} while (0);
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(fmt, ...) do { \
  if (common::GetLogLevel() <= common::kLogLevelError) { \
    fprintf(stderr, "[ERROR] [%s:%d:%s()]" fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
  } \
} while (0);
#endif

#endif  // COMMON_LOG_H_
