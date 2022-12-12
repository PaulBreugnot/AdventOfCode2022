#include "utils.h"
#include <string>
#include <iostream>

std::vector<std::deque<char>> stacks;

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	stacks = parse_stacks(input);

	std::string line;
	for(;std::getline(input, line);){
		Move move = parse_move(line);
		for(std::size_t i = 0; i < move.count; i++) {
			char crate = stacks[move.from].front();
			stacks[move.from].pop_front();
			stacks[move.to].push_front(crate);
		}
	}
	
	for(auto& stack : stacks)
		std::cout << stack.front();
	std::cout << std::endl;
	return 0;
}
