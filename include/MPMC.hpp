import std;
#pragma once
#include <Constants.hpp>


template<typename T, typename Size, typename ConsumerCount, typename ProducerCount>
class MPMC
{
public:
    MPMC()
    {

    }
private:
    T* data_;

    alignas(CACHE_LINE_SIZE) std::atomic<size_t> head;
    alignas(CACHE_LINE_SIZE) std::atomic<size_t> tail;
    static_assert(std::atomic<size_t>::is_always_lock_free, "size_t is not lock free.");
};