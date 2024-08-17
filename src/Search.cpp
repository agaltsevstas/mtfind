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
        
        std::vector<std::pair<std::future<std::vector<Result>>, uint64_t>> future_results;
        // Можно было сделать параллельную обработку
        while (!_data->Empty())
        {
            auto [str, row] = _data->Pop();
            auto future_result = pool.AddTask(&KMP::Search, std::ref(_kmp), std::move(str));
            
            future_results.emplace_back(std::move(future_result), row);
        }
        
        std::vector<Result> results;
        try
        {
            for (auto& [future_result, row] : future_results)
            {
                auto result = future_result.get();
                for (auto& r : result)
                {
                    r._row = row + 1u;
                    results.emplace_back(std::move(r));
                }
            }
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
