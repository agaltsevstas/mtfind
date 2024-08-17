#pragma once

#include <cassert>
#include <future>
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
  
    template<class TFunction, class... TArgs>
    auto AddTask(TFunction&& function, TArgs&&... args) -> std::future<typename std::invoke_result_t<TFunction, TArgs...>>
    {
        using Result = typename std::invoke_result<TFunction, TArgs...>::type;

        auto task = std::make_shared<std::packaged_task<Result()>>
        (
            std::bind(std::forward<TFunction>(function), std::forward<TArgs>(args)...)
        );
        
        std::future<Result> result = task->get_future();
        {
            std::unique_lock lock(_mutex);
            assert(!_stop);
            _tasks.emplace([task]()
            {
                return (*task)();
            });
        }
        
        _cv.notify_one();
        return result;
    }
  
private:
    std::queue<std::function<void()>> _tasks;
    std::vector<std::thread> _threads;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _stop = false;
};
