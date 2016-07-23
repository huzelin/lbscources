#include <assert.h>
#include <signal.h>

#include "common/thread-fn.h"

namespace {

static void *ThreadRoutine(void *pArg) {
  // Following code will guarantee more predictable latencies as it'll
  // disallow any signal handling in the I/O thread.
  sigset_t signal_set;
  int rc = sigfillset(&signal_set);
  assert(rc == 0);
  rc = pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
  assert(rc == 0);

  common::ThreadFn *self = (common::ThreadFn *)pArg;
  self->m_tfn(self->m_pArg);

  return NULL;
}

}

namespace common {

ThreadFn::~ThreadFn() {
  Stop();
}

void ThreadFn::Start(thread_fn *tfn, void *pArg) {
  m_tfn = tfn;
  m_pArg = pArg;
  pthread_t tid;
  int rc = pthread_create(&tid, NULL, ThreadRoutine, this);
  assert(rc == 0);
  m_threadDescriptor.push_back(tid);
}

void ThreadFn::Stop() {
  for (std::vector<pthread_t>::iterator iter = m_threadDescriptor.begin();
       iter != m_threadDescriptor.end(); ++iter) {
    int rc = pthread_join(*iter, NULL);
    assert(rc == 0);
  }
}

}  // namespace common
