#include "common/log.h"

namespace common {

static int g_log_level = kLogLevelNone;

void SetLogLevel(int log_level) {
  g_log_level = log_level;
}

int GetLogLevel() {
  return g_log_level;
}

}  // namespace common
