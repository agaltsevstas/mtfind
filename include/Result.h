#pragma once

#include <iostream>
#include <string>


struct Result
{
    inline bool operator<(const Result& other) const noexcept
    {
        return _row < other._row || (_row == other._row && _column < other._column);
    }
    
    uint64_t _row;
    uint64_t _column;
    std::string _match;
};

inline std::ostream& operator<<(std::ostream &os, const Result& other)
{
    return std::cout << other._row << " " << other._column << " " << other._match;
}
