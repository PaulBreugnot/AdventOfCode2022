#include "utils.h"
#include <limits>
#include <regex>
#include <iostream>

const std::regex point_regex {"(\\d+),(\\d+)"};

Cell& World::cell(std::size_t x, std::size_t y) {
	return _cells[y-origin[1]][x-origin[0]];
}

const std::vector<std::vector<Cell>>& World::cells() const {
	return _cells;
}

std::size_t World::numSand() const {
	return num_sand;
}

bool World::in_range(const Point& p) const {
	return !(p[0] < origin[0] || p[0] > extent[0] || p[1] < origin[1] || p[1] > extent[1]);
}

bool World::fall() {
	bool sleep = false;
	Point sand = {source[0], source[1] + 1};
	if(cell(sand[0], sand[1]) != AIR)
		return false;
	while(!sleep) {
		if(in_range({sand[0], sand[1]+1})) {
			if(cell(sand[0], sand[1]+1) == AIR) {
				sand[1]++;
			} else {
				if (in_range({sand[0]-1, sand[1]+1})) {
					if(cell(sand[0]-1, sand[1]+1) == AIR) {
						sand = {sand[0]-1, sand[1]+1};
					} else {
						if (in_range({sand[0]+1, sand[1]+1})) {
							if(cell(sand[0]+1, sand[1]+1) == AIR) {
								sand = {sand[0]+1, sand[1]+1};
							} else {
								sleep = true;
							}
						} else {
							return false;
						}
					}
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
	}
	cell(sand[0], sand[1]) = SAND;
	return true;
}

void World::simulate() {
	while(fall()) {
		num_sand++;
	}
}

void World::load(std::ifstream& input) {
	std::vector<std::vector<Point>> rocks;
	origin = {
		std::numeric_limits<std::size_t>::max(),
		std::numeric_limits<std::size_t>::max()
	};
	extent = {0, 0};

	std::string line;
	for(;std::getline(input, line);) {
		rocks.emplace_back();
		std::smatch match;
		const std::string cline = line;
		auto it = cline.begin();
		while(std::regex_search(it, cline.end(), match, point_regex)) {
			Point p = {std::stoul(match[1]), std::stoul(match[2])};
			if(p[0] < origin[0])
				origin[0] = p[0];
			else if(p[0] > extent[0])
				extent[0] = p[0];
			if(p[1] < origin[1])
				origin[1] = p[1];
			else if(p[1] > extent[1])
				extent[1] = p[1];
			rocks.back().emplace_back(p);
			it = match[0].second;
		}
	}
	if(source[0] < origin[0])
		origin[0] = source[0];
	else if(source[0] > extent[0])
		extent[0] = source[0];
	if(source[1] < origin[1])
		origin[1] = source[1];
	else if(source[1] > extent[1])
		extent[1] = source[1];

	std::cout << "Origin: " << origin[0] << "," << origin[1] << std::endl;
	std::cout << "Extent: " << extent[0] << "," << extent[1] << std::endl;

	for(int y = 0; y <= extent[1]-origin[1]; y++) {
		_cells.emplace_back();
		for(int x = 0; x <= extent[0]-origin[0]; x++)
			_cells.back().emplace_back(AIR);
	}
	_cells[source[1]-origin[1]][source[0]-origin[0]] = SOURCE;

	for(auto line : rocks) {
		auto start = line.begin();
		auto end = line.begin();
		end++;
		while(end != line.end()) {
			Point start_point = *start;
			Point end_point = *end;
			//std::cout << start_point[0] << "," << start_point[1] << " -> "
				//<< end_point[0] << "," << end_point[1] << std::endl;
			if(start_point[0] < end_point[0]) {
				for(int x = start_point[0]; x <= end_point[0]; x++) {
					//std::cout << " (" << start_point[1]-origin[1] << "," << x-origin[0] << ")"
						//<< std::endl;
					cell(x, start_point[1]) = ROCK;
				}
			} else if (start_point[0] > end_point[0]) {
				for(int x = end_point[0]; x <= start_point[0]; x++) {
					//std::cout << " (" << start_point[1]-origin[1] << "," << x-origin[0] << ")"
						//<< std::endl;
					cell(x, start_point[1]) = ROCK;
				}
			}
			if(start_point[1] < end_point[1]) {
				for(int y = start_point[1]; y <= end_point[1]; y++) {
					//std::cout << " (" << y-origin[1] << "," << start_point[0]-origin[0] << ")"
						//<< std::endl;
					cell(start_point[0], y) = ROCK;
				}
			} else if(start_point[1] > end_point[1]) {
				for(int y = end_point[1]; y <= start_point[1]; y++) {
					//std::cout << " (" <<  y-origin[1] << "," << start_point[0]-origin[0] << ")"
						//<< std::endl;
					cell(start_point[0], y) = ROCK;
				}
			}
			
			start++;
			end++;
		}
	}
}

std::ostream& operator<<(std::ostream& io, const World& w) {
	for(auto line : w.cells()) {
		for(auto cell : line) {
			switch(cell) {
				case AIR:
					io << '.';
					break;
				case ROCK:
					io << '#';
					break;
				case SAND:
					io << 'O';
					break;
				case SOURCE:
					io << '+';
			}
		}
		io << std::endl;
	}
	return io;
}
