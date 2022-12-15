#include "utils.h"
#include <fstream>
#include <set>
#include <iostream>

std::vector<Point> knots {10};
std::set<Point> visited_points;

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	std::string line;
	
	for(int i = 0; i < 10; i++)
		knots[i] = {0, 0};
	for(;std::getline(input, line);) {
		Move m = parse_move(line);
		for(int i = 0; i < m.count; i++) {
			move(knots[0], m);
			for(std::size_t j = 0; j < knots.size()-1; j++)
				follow(knots[j], knots[j+1]);
			visited_points.insert(knots.back());
		}
	}
	std::cout << visited_points.size() << std::endl;
	return 0;
}
