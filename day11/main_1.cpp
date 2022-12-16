#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	
	std::cout << sizeof(int) << std::endl;
	auto input = std::ifstream(argv[1]);
	std::vector<Monkey> monkeys = parse(input);

	for(int i = 0; i < 20; i++) {
		round(monkeys);
	}
	for(auto& monkey : monkeys)
		std::cout << "Monkey " << monkey.id << ":" << monkey.test_count << std::endl;
	return 0;
}
