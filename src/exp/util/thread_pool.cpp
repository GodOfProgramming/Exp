#include "thread_pool.hpp"

namespace Exp
{
  namespace Util
  {
    ThreadPriority& operator++(ThreadPriority& tp)
    {
      return tp = static_cast<ThreadPriority>(static_cast<int>(tp) + 1);
    }

    ThreadPriority operator++(ThreadPriority& tp, int)
    {
      auto res = tp;
      ++tp;
      return res;
    }

    ThreadPool::ThreadPool(std::size_t tc)
    {
      if (tc == 0) {
        LOG(FATAL) << "Cannot use a thread pool with 0 workers";
      }
      dispatcher = std::make_shared<WorkerThread>();
      dispatcher->exec(std::bind(&ThreadPool::dispatch, this));

      this->workers.resize(tc);
      for (auto& worker : this->workers) { worker = std::make_unique<WorkerThread>(this->dispatcher); }
    }

    ThreadPool::~ThreadPool()
    {
      this->stop();
    }

    void ThreadPool::enqueue(ThreadPriority tp, std::function<void(void)>&& fn)
    {
      this->priority_queue[tp].push(std::move(fn));
      this->dispatcher->rerun();
    }

    void ThreadPool::stop()
    {
      this->dispatcher->stop();
      for (auto& worker : this->workers) { worker->stop(); }
    }

    void ThreadPool::dispatch()
    {
      for (ThreadPriority pri = ThreadPriority::NON; pri <= ThreadPriority::TOP; pri++) {
        auto& queue = this->priority_queue[pri];
        if (!queue.empty()) {
          this->try_assign(queue);
        }
      }
    }

    void ThreadPool::try_assign(FnQueue& queue)
    {
      for (auto& worker : this->workers) {
        if (worker->done()) {
          auto fn = std::move(queue.back());
          queue.pop();
          worker->exec(std::move(fn));
        }
        if (queue.empty()) {
          break;
        }
      }
    }
  }  // namespace Util
}  // namespace Exp
