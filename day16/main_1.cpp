#include "utils.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);

	World world(input, 30, 1);

	int result = world.solve();
	std::cout << std::endl;
	std::cout << "Max pressure released: " << result << std::endl;
	return 0;
}
