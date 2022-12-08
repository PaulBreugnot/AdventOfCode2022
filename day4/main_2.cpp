#include <fstream>
#include <iostream>

#include "utils.h"


int main(int argc, char *argv[]) {
	int count = 0;
	auto input = std::ifstream(argv[1]);
	for(std::string line ; std::getline(input, line);) {
		Pair<Range> ranges = parse(line);
		if(
				in_range(ranges[0], ranges[1]) ||
				in_range(ranges[1], ranges[0]) ||
				in_range(ranges[0][0], ranges[1]) && !in_range(ranges[0][1], ranges[1]) ||
				in_range(ranges[1][0], ranges[0]) && !in_range(ranges[1][1], ranges[0]))
			count++;
	}
	std::cout << "Nombre: " << count << std::endl;
	return 0;
}
