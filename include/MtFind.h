#pragma once

#include "Queue.h"


#include <string>
#include <vector>


class Result;
using Data = Queue<std::pair<std::string, uint64_t>>;


class MtFind
{
public:
    std::vector<Result> CalculateResults(const std::string& iMask, const std::shared_ptr<Data>& iData);
};
