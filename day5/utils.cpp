#include "utils.h"
#include <regex>

std::vector<std::deque<char>> parse_stacks(std::ifstream& input) {
	std::vector<std::deque<char>> stacks;
	std::string line;
	std::getline(input, line);
	// Loop until the empty line
	while(line.size() > 0) {
		for(std::size_t i = 0; i < line.size(); i++) {
			if(std::isalpha(line[i])) {
				std::size_t index = i/4;
				if(stacks.size() < index+1)
					stacks.resize(index+1);
				stacks[index].push_back(line[i]);
			}
		}
		std::getline(input, line);
	}
	return stacks;
}

Move parse_move(std::string line) {
	static const std::regex move_regex {"move ([[:digit:]]+) from ([[:digit:]]+) to ([[:digit:]]+)"};
	std::smatch match;
	std::regex_match(line, match, move_regex);

	Move move;
	move.count = std::stoi(match[1]);
	move.from = std::stoi(match[2])-1;
	move.to = std::stoi(match[3])-1;

	return move;
}
