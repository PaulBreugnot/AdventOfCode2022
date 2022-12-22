#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	
	std::ifstream input = std::ifstream(argv[1]);
	World world(input);
	for(auto sensor : world.getSensors())
		std::cout << sensor << std::endl;
	std::cout << std::endl;
	std::cout << world.impossible_beacons_count(std::stoi(argv[2])) << std::endl;
	return 0;
}
