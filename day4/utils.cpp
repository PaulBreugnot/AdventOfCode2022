#include "utils.h"

Pair<Range> parse(std::string line) {
	std::size_t split_pair = line.find(',');
	std::array<std::string, 2> pairs {
		line.substr(0, split_pair),
		line.substr(split_pair+1)
	};

	Pair<Range> result;
	for(int i = 0; i < 2; i++) {
		std::size_t split_range = pairs[i].find('-');
		result[i] = {
			std::stoi(pairs[i].substr(0, split_range)),
			std::stoi(pairs[i].substr(split_range+1))
		};
	}
	return result;
}

bool in_range(Range range1, Range range2) {
	return range1[0] >= range2[0] && range1[1] <= range2[1];
}

bool in_range(int i, Range range) {
	return i >= range[0] && i <= range[1];
}
