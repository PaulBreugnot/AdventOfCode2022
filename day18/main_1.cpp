#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);

	World world;
	world.parse(input);
	std::cout << "Lava drop surface: " << world.surface() << std::endl;
	return 0;
}
