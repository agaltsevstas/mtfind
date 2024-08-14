#include "KMP.h"
#include "MtFind.h"
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
    
    std::vector<Result> CalculateResults()
    {
        ThreadPool pool;
        std::vector<Result> results;
        
        // Можно было сделать параллельную обработку
        try
        {
            while (!_data->Empty())
            {
                auto [str, row] = _data->Pop();
                pool.AddTask(std::bind(&KMP::Search, std::ref(_kmp), std::move(str), row, ref(results)));
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


std::vector<Result> MtFind::CalculateResults(const std::string& mask, const std::shared_ptr<Data>& data)
{
    return KMPDecorator(mask, data).CalculateResults();
}
