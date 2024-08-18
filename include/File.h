#pragma once

#include "Forward.h"


#include <fstream>
#include <string>
#include <thread>


class File
{
public:
    File(const std::string& iFileName);
    ~File();
    
    inline std::shared_ptr<Data> GetData() const noexcept
    {
        return _data;
    }
    
private:
    void Read();
    
private:
    std::ifstream _file;
    std::shared_ptr<Data> _data;
    std::thread _thread;
};
