#include <fstream>
#include <iostream>
#include "utils.h"

int main(int argc, char *argv[])
{
	int total_priority = 0;
	auto input = std::ifstream(argv[1]);
	for(std::string line ; std::getline(input, line);) {
		Compartments compartments = {
			line.substr(0, line.size()/2),
			line.substr(line.size()/2, line.size()/2)
		};
		for(char item : compartments[0]) {
			if(compartments[1].find(item) != std::string::npos) {
				total_priority += priority(item);
				break;
			}
		}
	}
	std::cout << "Total priority: " << total_priority << std::endl;
	return 0;
}
