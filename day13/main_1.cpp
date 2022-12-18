#include "utils.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	int sum = 0;
	int i = 0;
	std::string line;
	for(;std::getline(input, line);) {
		if(line.size() > 0) {
			i++;
			std::string line1 = line;
			std::string line2;
			std::getline(input, line2);
			auto* packet1 = parse_packet(line1);
			auto* packet2 = parse_packet(line2);
			std::cout << i << ": " << line1 << " vs " << line2 << std::endl;
			if(compare(*packet1, *packet2) <= 0) {
				std::cout << "  Right order" << std::endl;
				sum+=i;
			} else {
				std::cout << "  Bad order" << std::endl;
			}
			delete packet1;
			delete packet2;
		}
	}
	std::cout << "Sum : " << sum << std::endl;
	return 0;
}
