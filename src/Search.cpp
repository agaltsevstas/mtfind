#include "KMP.h"
#include "Search.h"
#include "Queue.h"
#include "Result.h"
#include "ThreadPool.h"


#include <iostream>
#include <memory>


class KMPDecorator
{
public:
    KMPDecorator(const std::string& mask, const std::shared_ptr<Data>& data):
    _kmp(mask),
    _data([&data]
    {
        if (!data)
            throw std::runtime_error("Пустые данные!");
        return data;
    }())
    {}
    
    std::vector<Result> SearchResults()
    {
        ThreadPool pool;
        
        std::vector<std::future<void>> futures;
        std::vector<Result> results;
        std::mutex mutex;
        
        // Можно было сделать параллельную обработку
        while (!_data->Empty())
        {
            auto [str, row] = _data->Pop();
            auto future = pool.AddTask(&KMP::Search, std::ref(_kmp), std::move(str), [&mutex, &results, row](uint64_t column, std::string&& match)
            {
                std::lock_guard lock(mutex);
                results.emplace_back(row + 1u, column, std::move(match));
            });
            
            futures.emplace_back(std::move(future));
        }
        
        try
        {
            for (auto& future : futures)
                future.get();
        }
        catch (const std::exception& exception)
        {
            std::cerr << "Исключение: " << exception.what() << ", поток: " << std::this_thread::get_id() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Неизвестная ошибка! Поток: " << std::this_thread::get_id() << std::endl;
        }
        
        std::sort(results.begin(), results.end());
        return results;
    }
    
private:
    KMP _kmp;
    const std::shared_ptr<Data> _data;
};


std::vector<Result> Search::SearchResults(const std::string& mask, const std::shared_ptr<Data>& data)
{
    return KMPDecorator(mask, data).SearchResults();
}
