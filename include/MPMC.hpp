import std;
#pragma once
#include <Constants.hpp>

template<typename DataType, size_t N = 0, typename IndexType = std::uint32_t>
class MPMC
{
    using value_type = DataType;
    using index_type = IndexType;
    static_assert(std::is_trivial_v<value_type>, "value_type must be trivial.");
    static_assert(std::is_unsigned_v<index_type>, "index_type must be unsigned");
    static_assert(std::atomic<value_type>::is_always_lock_free, "value_type is not lock free.");
    static_assert(std::atomic<index_type>::is_always_lock_free, "index_type is not lock free.");
    static_assert(std::popcount(N)==1, "size_t N must be a power of 2.");
public:
    MPMC(const int n = N)
    {
    }
private:
    std::array<value_type, N> data_;
    alignas(CACHE_LINE_SIZE) std::atomic<index_type> read_index_;
    alignas(CACHE_LINE_SIZE) std::atomic<index_type> write_index_;
};