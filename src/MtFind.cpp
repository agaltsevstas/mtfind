#include "KMP.h"
#include "MtFind.h"
#include "Queue.h"
#include "Result.h"
#include "ThreadPool.h"


#include <iostream>
#include <memory>


class KMPDecorator : public KMP
{
public:
    KMPDecorator(const std::string& mask, const std::shared_ptr<Data>& data):
    KMP(mask),
    _data([&data]
    {
        if (!data)
            throw std::runtime_error("Пустые данные!");
        return data;
    }())
    {}
    
    std::vector<Result> CalculateResults()
    {
        CalculateLPS();
        ThreadPool pool;
        
        std::vector<Result> results;
        
        // Можно было сделать параллельную обработку
        while (!_data->Empty())
        {
            const auto& [str, row] = _data->Pop();
            pool.AddTask(std::bind(&KMP::Search, this, str, row, std::ref(results)));
        }
        
        std::sort(results.begin(), results.end());
        return results;
    }
    
private:
    const std::shared_ptr<Data> _data;
};


std::vector<Result> MtFind::CalculateResults(const std::string& mask, const std::shared_ptr<Data>& data)
{
    return KMPDecorator(mask, data).CalculateResults();
}
