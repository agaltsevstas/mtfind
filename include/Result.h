#pragma once

#include <string>

class Result
{
public:
    Result(uint64_t row, uint64_t column, std::string&& match);
    bool operator<(const Result& other) const noexcept;
    friend std::ostream& operator<<(std::ostream &os, const Result& other);
    
private:
    uint64_t _row;
    uint64_t _column;
    std::string _match;
};
