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
		std::deque<char> grab;
		for(std::size_t i = 0; i < move.count; i++) {
			grab.push_back(stacks[move.from].front());
			stacks[move.from].pop_front();
		}
		for(std::size_t i = 0; i < move.count; i++) {
			stacks[move.to].push_front(grab.back());
			grab.pop_back();
		}
	}
	
	for(auto& stack : stacks)
		std::cout << stack.front();
	std::cout << std::endl;
}
