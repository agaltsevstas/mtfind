#include "Result.h"

#include <iostream>


Result::Result(uint64_t row, uint64_t column, std::string&& match) :
_row(row),
_column(column),
_match(std::move(match))
{
    
}

bool Result::operator<(const Result& other) const noexcept
{
    return _row < other._row || (_row == other._row && _column < other._column);
}
  
std::ostream& operator<<(std::ostream &os, const Result& other)
{
    return std::cout << other._row << " " << other._column << " " << other._match;
}
