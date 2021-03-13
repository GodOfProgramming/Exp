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
     : priority_queue([](const PrioritizedFn& left, const PrioritizedFn& right) { return left.first > right.first; })
     , dispatcher(std::make_shared<WorkerThread>())
    {
      if (tc == 0) {
        LOG(FATAL) << "Cannot use a thread pool with 0 workers";
      }

      dispatcher->exec(std::bind(&ThreadPool::dispatch, this));

      this->workers.resize(tc);
      for (auto& worker : this->workers) { worker = std::make_unique<WorkerThread>(this->dispatcher); }
    }

    ThreadPool::~ThreadPool()
    {
      this->stop();
    }

    void ThreadPool::enqueue(ThreadPriority tp, std::function<void(void)> fn)
    {
      std::unique_lock<std::mutex> lk(this->m);
      this->priority_queue.push(std::make_pair(tp, fn));
      lk.unlock();
      this->dispatcher->rerun();
    }

    void ThreadPool::stop()
    {
      this->dispatcher->stop();
      for (auto& worker : this->workers) { worker->stop(); }
    }

    void ThreadPool::dispatch()
    {
      std::lock_guard<std::mutex> lk(this->m);
      if (!this->priority_queue.empty()) {
        for (auto& worker : this->workers) {
          if (worker->done()) {
            auto fn = this->priority_queue.top();
            this->priority_queue.pop();
            worker->exec(fn.second);
          }
          if (this->priority_queue.empty()) {
            break;
          }
        }
      }
    }
  }  // namespace Util
}  // namespace Exp
