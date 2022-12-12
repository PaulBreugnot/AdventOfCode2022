#include <vector>
#include <deque>
#include <fstream>

std::vector<std::deque<char>> parse_stacks(std::ifstream& input);

struct Move {
	int count;
	int from;
	int to;
};

Move parse_move(std::string line);
