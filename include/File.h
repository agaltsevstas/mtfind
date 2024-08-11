#pragma once

#include "Queue.h"

#include <fstream>
#include <string>
#include <thread>


class File
{
    using Data = Queue<std::pair<std::string, uint64_t>>;
public:
    File(const std::string& iFileName);
    ~File();
    
    std::shared_ptr<Data> GetData() const noexcept;
    
private:
    void Read();
    
private:
    std::ifstream _file;
    std::shared_ptr<Data> _data;
    std::thread _thread;
};
