#include "utils.h"
#include <fstream>
#include <set>
#include <iostream>

Point head_location = {0, 0};
Point tail_location = {0, 0};

std::set<Point> visited_points;

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	std::string line;

	for(;std::getline(input, line);) {
		Move m = parse_move(line);
		for(int i = 0; i < m.count; i++) {
			move(head_location, m);
			follow(head_location, tail_location);
			visited_points.insert(tail_location);
		}
	}
	std::cout << visited_points.size() << std::endl;
	return 0;
}
