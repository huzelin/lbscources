#ifndef COMMON_THREAD_FN_
#define COMMON_THREAD_FN_

#include <pthread.h>
#include <vector>

namespace common {

typedef void (thread_fn) (void *pArg);

class ThreadFn {
 public:
  ThreadFn () { }
  virtual ~ThreadFn();

  void Start(thread_fn *tfn, void *pArg);
  void Stop();

  thread_fn *m_tfn;
  void *m_pArg;

 private:
  ThreadFn(const ThreadFn &);
  ThreadFn &operator=(const ThreadFn &);
  std::vector<pthread_t> m_threadDescriptor;
};

}  // namespace common

#endif // COMMON_THREAD_FN_
