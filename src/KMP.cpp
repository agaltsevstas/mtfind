#include "KMP.h"
#include "Result.h"


#include <iostream>
#include <exception>
#include <thread>


KMP::KMP(const std::string& mask):
_mask(mask)
{
    CalculateLPS();
}

void KMP::CalculateLPS()
{
    const uint64_t mask_size = _mask.size();
    _lps.resize(mask_size);
 
    uint64_t i = 1;
    uint64_t len = 0;
    
    while (i < mask_size)
    {
        if (_mask[i] != '?' && _mask[i] == _mask[len])
        {
            ++len;
            _lps[i] = len;
            ++i;
        }
        else
        {
            if (len != 0)
            {
                len = _lps[len - 1u];
            }
            else
            {
                _lps[i] = 0;
                ++i;
            }
        }
    }
}

void KMP::Search(const std::string& str, uint64_t row, std::vector<Result>& results)
{
    try
    {
        uint64_t str_index = 0;
        uint64_t mask_index = 0;
        
        uint64_t str_size = str.size();
        uint64_t mask_size = _mask.size();
        
        while ((str_size - str_index) >= (mask_size - mask_index))
        {
            if (_mask[mask_index] == '?' || _mask[mask_index] == str[str_index])
            {
                ++mask_index;
                ++str_index;
            }
     
            if (mask_index == mask_size)
            {
                {
                    std::unique_lock lock(_mutex);
                    results.emplace_back(row + 1u, str_index - mask_index + 1u, str.substr(str_index - mask_index, _mask.length()));
                }
                
                mask_index = _lps[mask_index - 1];
            }
            else if ((str_index < str_size) && (_mask[mask_index] != str[str_index]) && (_mask[mask_index] != '?'))
            {
                if (mask_index != 0)
                    mask_index = _lps[mask_index - 1];
                else
                    ++str_index;
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
}
