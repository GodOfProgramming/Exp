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

    void WorkerThread::exec(std::function<void(void)>&& f)
    {
      std::lock_guard<std::mutex> lk(this->m);
      this->fn = std::move(f);
    }

    auto WorkerThread::done() const noexcept -> bool
    {
      return this->is_done;
    }

    void WorkerThread::rerun()
    {
      std::lock_guard<std::mutex> lk(this->m);
      this->cv.notify_one();
    }

    void WorkerThread::stop()
    {
      std::unique_lock<std::mutex> lk(this->m);
      lk.lock();
      if (this->worker.joinable()) {
        this->running = false;
        lk.unlock();
        this->cv.notify_one();
        this->worker.join();
      }
    }

    void WorkerThread::run_loop()
    {
      while (running) {
        std::unique_lock<std::mutex> lk(this->m);
        cv.wait(lk);
        lk.lock();
        if (!this->is_done || running) {
          this->is_done = false;
          this->fn();
          this->is_done = true;
          if (this->handler) {
            this->handler->rerun();
          }
        }
      }
    }
  }  // namespace Util
}  // namespace Exp