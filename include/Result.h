#pragma once

#include <iostream>
#include <string>

class Result
{
public:
    Result(uint64_t row, uint64_t column, std::string&& match):
    _row(row),
    _column(column),
    _match(std::move(match))
    {

    }
    
    inline bool operator<(const Result& other) const noexcept
    {
        return _row < other._row || (_row == other._row && _column < other._column);
    }
    
    friend inline std::ostream& operator<<(std::ostream &os, const Result& other)
    {
        return std::cout << other._row << " " << other._column << " " << other._match;
    }
    
private:
    uint64_t _row;
    uint64_t _column;
    std::string _match;
};
