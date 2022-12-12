#include "utils.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	std::string line;
	std::getline(input, line);
	std::cout << start_of_message(line) << std::endl;

	return 0;
}
