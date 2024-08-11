#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


class ThreadPool
{
public:
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency())
    {
        for (decltype(num_threads) i = 0; i < num_threads; ++i)
        {
            _threads.emplace_back([this]
            {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock lock(_mutex);
                        _cv.wait(lock, [this] { return !_tasks.empty() || _stop; });
  
                        if (_stop && _tasks.empty())
                            return;
  
                        task = std::move(_tasks.front());
                        _tasks.pop();
                    }
  
                    task();
                }
            });
        }
    }
  
    ~ThreadPool()
    {
        {
            std::unique_lock lock(_mutex);
            _stop = true;
        }
  
        _cv.notify_all();
  
        for (auto& thread : _threads)
            thread.join();
    }
  
    void AddTask(std::function<void()> task)
    {
        {
            std::unique_lock lock(_mutex);
            _tasks.emplace(std::move(task));
        }
        
        _cv.notify_one();
    }
  
private:
    std::queue<std::function<void()>> _tasks;
    std::vector<std::thread> _threads;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _stop = false;
};
