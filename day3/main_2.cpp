#include <fstream>
#include <iostream>

#include "utils.h"

int main(int argc, char *argv[])
{
	int badge_priorities = 0;
	auto input = std::ifstream(argv[1]);
	std::array<Ruckpack, 3> group;
	int i = 0;
	for(std::string line ; std::getline(input, line);) {
		group[i++] = line;
		if(i == 3) {
			i = 0;
			for(char item : group[0]) {
				if(group[1].find(item) != std::string::npos
						&& group[2].find(item) != std::string::npos) {
					// Badge found!
					badge_priorities += priority(item);
					break;
				}
			}
		}
	}
	std::cout << "Sum of badge priorities: " << badge_priorities << std::endl;
	return 0;
}

