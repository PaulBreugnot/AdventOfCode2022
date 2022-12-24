#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	
	auto input = std::ifstream(argv[1]);
	World world;
	world.parse(input);

	std::size_t surface =  world.outerSurface();
	std::cout << "Outer lava drop surface: " << surface << std::endl;
	return 0;
}
