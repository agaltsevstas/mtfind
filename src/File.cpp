#include "File.h"

#include <iostream>


File::File(const std::string& iFileName):
_file(iFileName),
_data(std::make_shared<Data>())
{
    if (!_file.is_open())
        throw std::runtime_error("Невозможно открыть файл >> " + iFileName);
    
    if (_file.peek() == EOF)
        throw std::runtime_error("Пустой файл >> " + iFileName);
    
    _data->SetStop(false);
    _thread = std::thread(&File::Read, this);
}

File::~File()
{
    _thread.join();
    _file.close();
}

void File::Read()
{
    std::string line;
    uint64_t row = 0;
    
    while (getline(_file, line))
        _data->Push(line, row++);
    
    _data->SetStop(true);
}

std::shared_ptr<File::Data> File::GetData() const noexcept
{
    return _data;
}
