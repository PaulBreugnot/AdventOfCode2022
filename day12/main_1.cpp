#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	auto world = parse_world(input);
	for(auto line : world.map) {
		for(auto col : line)
			if(col < 10)
				std::cout << col << " ";
			else
				std::cout << col << "";
		std::cout << std::endl;
	}
	Path shortest_path = A_star().solve(world);
	if(shortest_path.size() == 0)
		std::cout << "Not found!" << std::endl;
	else
		std::cout << "Shortest path length: " << shortest_path.size() << std::endl;
	return 0;
}
