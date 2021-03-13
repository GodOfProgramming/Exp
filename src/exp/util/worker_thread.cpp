#include "worker_thread.hpp"

namespace Exp
{
  namespace Util
  {
    WorkerThread::WorkerThread()
     : WorkerThread(nullptr)
    {}

    WorkerThread::WorkerThread(std::shared_ptr<WorkerThread> h)
     : worker(std::bind(&WorkerThread::run_loop, this))
     , handler(h)
    {}

    WorkerThread::~WorkerThread()
    {
      this->stop();
    }

    void WorkerThread::exec(std::function<void(void)> f)
    {
      std::unique_lock<std::mutex> lk(this->m);
      this->fn      = f;
      this->is_done = false;
      lk.unlock();
      this->cv.notify_one();
    }

    auto WorkerThread::done() const noexcept -> bool
    {
      return this->is_done;
    }

    void WorkerThread::rerun()
    {
      std::unique_lock<std::mutex> lk(this->m);
      this->is_done = false;
      lk.unlock();
      this->cv.notify_one();
    }

    void WorkerThread::stop()
    {
      std::unique_lock<std::mutex> lk(this->m);
      if (this->worker.joinable()) {
        this->running = false;
        lk.unlock();
        this->cv.notify_one();
        this->worker.join();
      }
    }

    void WorkerThread::run_loop()
    {
      do {
        std::unique_lock<std::mutex> lk(this->m);
        cv.wait(lk, [this] { return !this->is_done || !this->running; });
        if (running) {
          this->fn();
          this->is_done = true;
          lk.unlock();
          if (this->handler) {
            this->handler->rerun();
          }
        }
      } while (running);
    }
  }  // namespace Util
}  // namespace Exp