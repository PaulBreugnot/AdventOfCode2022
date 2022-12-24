#include "utils.h"
#include <regex>
#include <limits>
#include <iostream>
#include <array>
#include <unordered_set>

const std::array<Point, 6> neighbor_directions {{
	{0, 0, 1},
	{0, 1, 0},
	{1, 0, 0},
	{0, 0, -1},
	{0, -1, 0},
	{-1, 0, 0}
}};

struct PointHash {
	std::size_t operator()(const Point& p) const {
		return std::hash<int_type>()(p.x+p.y+p.z);
	}
};

Point Point::operator+(const Point& p) const {
	return {x+p.x, y+p.y, z+p.z};
}

bool Point::operator==(const Point& p) const {
	return x==p.x && y==p.y && z==p.z;
}

std::array<Point, 6> Point::neighbors() const {
	std::array<Point, 6> _neighbors;
	for(int i = 0; i < 6; i++)
		_neighbors[i] = *this+neighbor_directions[i];

	return _neighbors;
}

std::ostream& operator<<(std::ostream& o, const Point& p) {
	o << "(" << p.x << "," << p.y << "," << p.z << ")";
	return o;
}

void World::setOrigin(const Point &origin) {
	this->origin = origin;
}

void World::setExtent(const Point &extent) {
	this->extent = extent;
}

World::Space World::buildSpace() const {
	Space space;
	space.resize(extent.x-origin.x+1);
	for(auto& y_line : space) {
		y_line.resize(extent.y-origin.y+1);
		for(auto& z_line : y_line)
			z_line.resize(extent.z-origin.z+1);
	}
	return space;
}

void World::parse(std::ifstream& input) {
	origin = {
		std::numeric_limits<int_type>::max(),
		std::numeric_limits<int_type>::max(),
		std::numeric_limits<int_type>::max()
	};
	extent = {
		std::numeric_limits<int_type>::min(),
		std::numeric_limits<int_type>::min(),
		std::numeric_limits<int_type>::min()
	};

	std::string line;
	std::smatch match;

	for(;std::getline(input, line);) {
		std::regex_match(line, match, std::regex {"(\\d+),(\\d+),(\\d+)"});
		Point p {
				std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3])
		};
		if(p.x < origin.x)
			origin.x = p.x;
		else if (p.x > extent.x)
			extent.x = p.x;
		if(p.y < origin.y)
			origin.y = p.y;
		else if (p.y > extent.y)
			extent.y = p.y;
		if(p.z < origin.z)
			origin.z = p.z;
		else if (p.z > extent.z)
			extent.z = p.z;

		points.push_back(p);
	}
	// Margin of one void cell in all direction to facilitate neighbors query
	origin.x--;
	origin.y--;
	origin.z--;
	extent.x++;
	extent.y++;
	extent.z++;

	std::cout << "Origin : " << origin << std::endl;
	std::cout << "Extent : " << extent << std::endl;
}

std::size_t World::surface(const std::vector<Point>& points) const {
	Space space = buildSpace();
	std::size_t surface = 0;
	for(const Point& p : points) {
		space[p.x-origin.x][p.y-origin.y][p.z-origin.z] = 1;
		int local_surface = 6;
		for(const Point& neighbor : p.neighbors()) {
			bool neighbor_in_box = true;
			// Assumes box size > 1 in all directions
			if(neighbor.x < origin.x) {
				neighbor_in_box = false;
				local_surface--;
			} else if(neighbor.x > extent.x) {
				neighbor_in_box = false;
				local_surface--;
			}
			if(neighbor.y < origin.y) {
				neighbor_in_box = false;
				local_surface--;
			} else if(neighbor.y > extent.y) {
				neighbor_in_box = false;
				local_surface--;
			}
			if(neighbor.z < origin.z) {
				neighbor_in_box = false;
				local_surface--;
			} else if(neighbor.z > extent.z) {
				neighbor_in_box = false;
				local_surface--;
			}
			if(neighbor_in_box)
				if(space[neighbor.x-origin.x][neighbor.y-origin.y][neighbor.z-origin.z] > 0)
					local_surface-=2;
		}
		//std::cout << "Local surface of " << p << ": " << local_surface << std::endl;
		surface+=local_surface;
	}

	return surface;
};

std::size_t World::surface() const {
	return surface(points);
};

std::size_t World::outerSurface() const {
	return outerSurface(points);
}

std::size_t World::outerSurface(const std::vector<Point>& points) const {
	Space inner_space = buildSpace();
	std::cout << "Lava cells count: " << points.size() << std::endl;
	for(const Point& p : points) {
		inner_space[p.x-origin.x][p.y-origin.y][p.z-origin.z] = 1;
	}
	Space outer_space = buildSpace();
	Space explored_space = buildSpace();
	std::size_t explored_count = 0;
	std::deque<Point> open_points;

	open_points.push_back(origin);
	explored_space[0][0][0] = 1;

	std::size_t all_volume = (extent.z-origin.z+1) * (extent.y-origin.y+1) * (extent.x-origin.x+1);
	std::size_t void_volume = all_volume - points.size();
	std::cout << "Building outer space (from " << void_volume << " void cells)..." << std::endl;
	while(!open_points.empty()) {
		const Point& current = open_points.front();
		++explored_count;
		for(const Point& neighbor : current.neighbors()) {
			if(neighbor.x>=origin.x && neighbor.x <= extent.x
					&& neighbor.y>=origin.y && neighbor.y <= extent.y
					&& neighbor.z>=origin.z && neighbor.z <= extent.z) {
				if(inner_space[neighbor.x-origin.x][neighbor.y-origin.y][neighbor.z-origin.z] == 0) {
					if(explored_space[neighbor.x-origin.x][neighbor.y-origin.y][neighbor.z-origin.z] == 0) {
						explored_space[neighbor.x-origin.x][neighbor.y-origin.y][neighbor.z-origin.z] = 1;
						open_points.push_back(neighbor);
					}
				}
			}
		}
		open_points.pop_front();
	}
	std::cout << "Done (" << explored_count << " reachable cells)." << std::endl;
	std::vector<Point> explored_points;
	for(int_type x = 0; x < explored_space.size(); x++) {
		for(int_type y = 0; y < explored_space[x].size(); y++) {
			for(int_type z = 0; z < explored_space[x][y].size(); z++) {
				if(explored_space[x][y][z] == 1)
					explored_points.push_back({x+origin.x, y+origin.y, z+origin.z});
			}
		}
	}
	return surface(explored_points);
}
