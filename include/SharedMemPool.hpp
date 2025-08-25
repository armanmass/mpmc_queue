import std;
#pragma once
#include <Constants.hpp>

template <typename T>
class SharedMemoryPool
{
public:
    SharedMemoryPool()
    { }
private:
    T* free_list_;
};