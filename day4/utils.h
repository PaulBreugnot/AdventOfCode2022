#include <array>
#include <string>

typedef std::array<int, 2> Range;

template<typename T>
using Pair = std::array<T, 2>;

Pair<Range> parse(std::string line);

bool in_range(Range range1, Range range2);
bool in_range(int i, Range range);
