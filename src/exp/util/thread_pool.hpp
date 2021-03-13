#pragma once

#include "worker_thread.hpp"

namespace Exp
{
  namespace Util
  {
    enum class ThreadPriority
    {
      NON,
      LOW,
      MED,
      HIGH,
      TOP,
    };

    class ThreadPool
    {
      using Fn            = std::function<void(void)>;
      using PrioritizedFn = std::pair<ThreadPriority, Fn>;

     public:
      ThreadPool(std::size_t tc);
      ~ThreadPool();

      void enqueue(ThreadPriority tp, Fn fn);

      void stop();

     private:
      using FnQueue = std::priority_queue<PrioritizedFn, std::deque<PrioritizedFn>, bool (*)(const PrioritizedFn& left, const PrioritizedFn& right)>;

      std::shared_ptr<WorkerThread> dispatcher;

      std::mutex m;
      FnQueue priority_queue;
      std::vector<std::unique_ptr<WorkerThread>> workers;

      void dispatch();

      void try_assign();
    };
  }  // namespace Util
}  // namespace Exp