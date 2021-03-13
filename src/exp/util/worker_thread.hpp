#pragma once

namespace Exp
{
  namespace Util
  {
    class WorkerThread
    {
     public:
      WorkerThread();
      WorkerThread(std::shared_ptr<WorkerThread> handler);

      ~WorkerThread();

      void exec(std::function<void(void)> f);

      auto done() const noexcept -> bool;

      void rerun();

      void stop();

     private:
      std::atomic<bool> running = true;
      std::atomic<bool> is_done = true;

      mutable std::mutex m;
      std::condition_variable cv;

      std::thread worker;
      std::function<void(void)> fn;

      std::shared_ptr<WorkerThread> handler;

      void run_loop();
    };
  }  // namespace Util
}  // namespace Exp