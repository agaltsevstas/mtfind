#pragma once

#include <queue>
#include <mutex>
#include <thread>


template <class T>
class Queue
{
public:
    template <class ...TArgs>
    void Push(TArgs&& ...args)
    {
        {
            std::lock_guard lock(_mutex);
            _queue.emplace(std::forward<TArgs>(args)...);
            ++_count;
        }
        
        _cv.notify_one();
    }

    T Pop()
    {
        std::unique_lock lock(_mutex);
        _cv.wait(lock, [this]() { return _count >= std::thread::hardware_concurrency() || _stop; }); // hardware_concurrency выдает у меня 8 потоков
        T item = std::move(_queue.front());
        _queue.pop();
        --_count;
        return item;
    }
    
    void SetStop(bool stop)
    {
        {
            std::lock_guard lock(_mutex);
            _stop = stop;
        }
        
        if (_stop)
            _cv.notify_one();
    }
    
    bool Empty()
    {
        std::lock_guard lock(_mutex);
        return _stop && _queue.empty();
    }
    
private:
    bool _stop = false;
    size_t _count = 0;
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _cv;
};

