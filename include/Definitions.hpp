import std;
using std::size_t;

static inline constexpr size_t CACHE_LINE_SIZE{ std::hardware_destructive_interference_size };

