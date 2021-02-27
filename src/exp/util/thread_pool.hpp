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
      using FnQueue = std::queue<std::function<void(void)>>;

     public:
      ThreadPool(std::size_t tc);
      ~ThreadPool();

      void enqueue(ThreadPriority tp, std::function<void(void)>&& fn);

      void stop();

     private:
      std::shared_ptr<WorkerThread> dispatcher;

      std::mutex m;
      std::map<ThreadPriority, FnQueue> priority_queue;

      std::vector<std::unique_ptr<WorkerThread>> workers;

      void dispatch();

      void try_assign(FnQueue& queue);
    };
  }  // namespace Util
}  // namespace Exp