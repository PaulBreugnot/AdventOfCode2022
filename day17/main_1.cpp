#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	World world(input);
	std::cout << world << std::endl;

	world.drop(2022);
	std::cout << "Final tower height: " << world.towerTop() << std::endl;
	
	return 0;
}
