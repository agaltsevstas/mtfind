#pragma once

#include <string>


struct Result
{
    bool operator<(const Result& other) const noexcept;
    friend std::ostream& operator<<(std::ostream &os, const Result& other);
    
    uint64_t _row;
    uint64_t _column;
    std::string _match;
};
