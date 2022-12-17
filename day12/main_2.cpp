#include "utils.h"
#include <iostream>
#include <limits>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	auto world = parse_world(input);
	int min_path_length = std::numeric_limits<int>::max();
	for(auto line : world.map) {
		for(auto col : line)
			if(col < 10)
				std::cout << col << " ";
			else
				std::cout << col << "";
		std::cout << std::endl;
	}
	for(int j = 0; j < world.map.size(); j++) {
		for(int i = 0; i < world.map[j].size(); i++) {
			if(world.map[j][i] == 0) {
				world.start = {j, i};
				int length = A_star().solve(world).size();
				if(length > 0 and length < min_path_length)
					min_path_length = length;
			}
		}
	}
	std::cout << "Shortest path length: " << min_path_length << std::endl;
	return 0;
}
