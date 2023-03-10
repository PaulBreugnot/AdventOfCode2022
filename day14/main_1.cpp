#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	SimpleWorld world(input);

	std::cout << world << std::endl;

	world.simulate();
	std::cout << world << std::endl;
	
	std::cout << world.numSand() << std::endl;
	return 0;
}
