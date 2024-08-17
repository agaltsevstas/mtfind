#include "File.h"
#include "Search.h"
#include "Result.h"

#include <iostream>
#include <exception>


int main(int argc, char* argv[])
{
    try
    {
        switch (argc)
        {
            case 0:
            case 1: throw std::runtime_error("Отсутствует файл");
            case 2: throw std::runtime_error("Отсутствует файл и маска");
        }
        File file(argv[1]);
        const auto results = Search().SearchResults(argv[2], file.GetData());
        
        std::cout << results.size() << std::endl;
        for (const auto& result : results)
            std::cout << result << std::endl;
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Неизвестная ошибка!" << std::endl;
    }
}
