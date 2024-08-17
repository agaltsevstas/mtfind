#pragma once


#include "Forward.h"


#include <string>
#include <vector>


struct Result;


// Stateless класс - подходящий для расширения
class Search
{
public:
    std::vector<Result> SearchResults(const std::string& iMask, const std::shared_ptr<Data>& iData);
};
