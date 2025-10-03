#include <atomic>
#include <cstdint>
#include <type_traits>
import std;
#pragma once
#include <Definitions.hpp>

template<typename DataType, size_t N = 1, typename IndexType = std::uint32_t>
class MPMC
{
public:
    using value_type = DataType;
    using index_type = IndexType;
    using uint128_t = unsigned __int128;

    static_assert(std::is_trivial_v<value_type>, "value_type must be trivial.");
    static_assert(std::is_unsigned_v<index_type>, "index_type must be unsigned");
    static_assert(std::atomic<value_type>::is_always_lock_free, "value_type is not lock free.");
    static_assert(std::atomic<index_type>::is_always_lock_free, "index_type is not lock free.");
    static_assert(std::popcount(N)==1, "size_t N must be a power of 2.");

private:
    struct alignas(8) helper_type
    {
        value_type data_;
        index_type index_;
    };

    template <size_t S>
    using unit_value_type = std::conditional_t<S==8,  uint64_t,  // if S==8
                            std::conditional_t<S==16, uint128_t, // else if S==16 
                                               void>>;           // else

    // combined entry for atmoic CAS
    using combined_type = typename unit_value_type<sizeof(helper_type)>::type;
    static_assert(!std::is_same_v<combined_type, void>, "combination of DataType and IndexType must be <= 16 bytes");

    // union entry to easily interpret upper and lower half of combined_value_
    union entry
    {
        combined_type combined_value_;
        struct indivdual
        {
            value_type data_;
            index_type seq_;
        };
    };

public:
    MPMC(const int n = N)
    { }

    void push(value_type val)
    {
        // TODO: relax increment memory ordering
        size_t ownedIdx = getIndex(write_index_++);
        data_[ownedIdx] = val;
    }

    void pop (value_type& val)
    {
        // TODO: relax increment memory ordering
        size_t ownedIdx = getIndex(read_index_++);
        val = data_[ownedIdx];
    }

private:
    static constexpr size_t getIndex(size_t idx) noexcept { return idx & (N-1); }

private:
    std::array<value_type, N> data_;
    alignas(CACHE_LINE_SIZE) std::atomic<index_type> read_index_;
    alignas(CACHE_LINE_SIZE) std::atomic<index_type> write_index_;
};