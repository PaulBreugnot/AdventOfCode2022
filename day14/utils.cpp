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

void World::simulate() {
	while(fall()) {
		num_sand++;
	}
}

void World::addRocks(const Point& start, const Point& end) {
	//std::cout << start_point[0] << "," << start_point[1] << " -> "
	//<< end_point[0] << "," << end_point[1] << std::endl;
	if(start[0] < end[0]) {
		for(int x = start[0]; x <= end[0]; x++) {
			//std::cout << " (" << start_point[1]-origin[1] << "," << x-origin[0] << ")"
			//<< std::endl;
			cell(x, start[1]) = ROCK;
		}
	} else if (start[0] > end[0]) {
		for(int x = end[0]; x <= start[0]; x++) {
			//std::cout << " (" << start_point[1]-origin[1] << "," << x-origin[0] << ")"
			//<< std::endl;
			cell(x, start[1]) = ROCK;
		}
	}
	if(start[1] < end[1]) {
		for(int y = start[1]; y <= end[1]; y++) {
			//std::cout << " (" << y-origin[1] << "," << start_point[0]-origin[0] << ")"
			//<< std::endl;
			cell(start[0], y) = ROCK;
		}
	} else if(start[1] > end[1]) {
		for(int y = end[1]; y <= start[1]; y++) {
			//std::cout << " (" <<  y-origin[1] << "," << start_point[0]-origin[0] << ")"
			//<< std::endl;
			cell(start[0], y) = ROCK;
		}
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
			addRocks(*start, *end);
				
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

SimpleWorld::SimpleWorld(std::ifstream& input) {
	load(input);
}

bool SimpleWorld::fall() {
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

FullWorld::FullWorld(std::ifstream& input) {
	load(input);
	for(int y = 0; y < 2; y++) {
		_cells.emplace_back(_cells.back().size());
		for(int x = 0; x < _cells[0].size(); x++) {
			_cells.back()[x] = AIR;
		}
	}
	addRocks({origin[0], extent[1]+2}, {extent[0], extent[1]+2});
}

void FullWorld::add_left_columns() {
	auto end = _cells.end();
	end--;
	for(auto it = _cells.begin(); it != end;  it++) {
		it->insert(it->begin(), column_increment, AIR);
	}
	// Floor
	end->insert(end->begin(), column_increment, ROCK);
	origin[0]-=column_increment;
}

void FullWorld::add_right_columns() {
	auto end = _cells.end();
	end--;
	for(auto it = _cells.begin(); it != end;  it++) {
		it->insert(it->end(), column_increment, AIR);
	}
	// Floor
	end->insert(end->end(), column_increment, ROCK);
	extent[0]+=column_increment;
}

bool FullWorld::fall() {
	bool sleep = false;
	Point sand = {source[0], source[1]};
	if(cell(sand[0], sand[1]) == SAND)
		// Only condition under which the simulation should stop
		return false;
	while(!sleep) {
		// Always in range because of the infinite ROCK floor
		if(cell(sand[0], sand[1]+1) == AIR) {
			sand[1]++;
		} else {
			if(!in_range({sand[0]-1, sand[1]+1})) {
				add_left_columns();
			}
			if(cell(sand[0]-1, sand[1]+1) == AIR) {
				sand = {sand[0]-1, sand[1]+1};
			} else {
				if (!in_range({sand[0]+1, sand[1]+1})) {
					add_right_columns();
				}
				if(cell(sand[0]+1, sand[1]+1) == AIR) {
					sand = {sand[0]+1, sand[1]+1};
				} else {
					sleep = true;
				}
			}
		}
	}
	cell(sand[0], sand[1]) = SAND;
	return true;
}

