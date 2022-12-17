#include "utils.h"
#include <cmath>
#include <set>
#include <limits>
#include <iostream>

World parse_world(std::ifstream& input) {
	World world;
	std::string line;
	int j = 0;
	for(;std::getline(input, line);) {
		world.map.resize(j+1);
		world.map.back().resize(line.size());
		for(int i = 0; i < line.size(); i++) {
			if(line[i] == 'S') {
				line[i] = 'a';
				world.start = {{j, i}};
			}
			if(line[i] == 'E') {
				line[i] = 'z';
				world.target = {{j, i}};
			}
			world.map[j][i] = (int) line[i] - 97;
		}
		j++;
	}
	return world;
}

std::vector<Point> World::neighbors(const Point& p) const {
	std::vector<Point> raw_neighbors;
	if(p[0] > 0)
		raw_neighbors.push_back({p[0]-1, p[1]});
	if(p[0] < map.size()-1)
		raw_neighbors.push_back({p[0]+1, p[1]});
	if(p[1] > 0)
		raw_neighbors.push_back({p[0], p[1]-1});
	if(p[1] < map[p[0]].size()-1)
		raw_neighbors.push_back({p[0], p[1]+1});

	std::vector<Point> neighbors;
	for(auto item : raw_neighbors)
		if(map[item[0]][item[1]] <= map[p[0]][p[1]]+1)
			neighbors.push_back(item);
	return neighbors;
}

float A_star::h(const World& world, const Point& p) const {
	return std::abs(p[1]-world.start[1]) + std::abs(p[0]-world.start[0]);
}

bool A_star::LowF::operator()(const Point &p1, const Point &p2) const {
	return a_star.f(p1) < a_star.f(p2);
}

float A_star::f(const Point& p) const {
	return _f[p[0]][p[1]];
}

void A_star::set_f(const Point& p, float value) {
	_f[p[0]][p[1]] = value;
}

float A_star::g(const Point& p) const {
	return _g[p[0]][p[1]];
}

void A_star::set_g(const Point& p, float value) {
	_g[p[0]][p[1]] = value;
}

void A_star::init_inf(const Map& map, std::vector<std::vector<float>>& scores) {
	scores.resize(map.size());
	for(int j = 0; j < scores.size(); j++) {
		scores[j].resize(map[j].size());
		for(int i = 0; i < scores[j].size(); i++) {
			scores[j][i] = std::numeric_limits<float>::infinity();
		}
	}
}
void A_star::init_came_from(const Map& map) {
	came_from.resize(map.size());
	for(int j = 0; j < came_from.size(); j++) {
		came_from[j].resize(map[j].size());
	}
}

Path A_star::build_path(const World& world) {
	Path path;
	Point current_node = world.target;
	while(current_node != world.start) {
		path.push_back(current_node);
		current_node = came_from[current_node[0]][current_node[1]];
	}
	return path;
}

Path A_star::solve(const World& world) {
	init_inf(world.map, _g);
	init_inf(world.map, _f);
	init_came_from(world.map);

	set_g(world.start, 0);
	set_f(world.start, h(world, world.start));

	std::multiset<Point, A_star::LowF> open_set(A_star::LowF {*this});
	open_set.insert(world.start);

	while(!open_set.empty()) {
		Point current = *open_set.begin();
		open_set.erase(open_set.begin());
		if (current == world.target)
			return build_path(world);

		std::cout << "Current: (" << current[0] << "," << current[1] << ")" << std::endl;
		for(Point neighbor : world.neighbors(current)) {
			std::cout << "  Neighbor: (" << neighbor[0] << ", " << neighbor[1] << ")" << std::endl;
			int current_distance = g(current) + 1;
			std::cout << "  d: " << current_distance << std::endl;
			std::cout << "  g(neighbor): " << g(neighbor) << std::endl;
			if (current_distance < g(neighbor)) {
				came_from[neighbor[0]][neighbor[1]] = current;
				set_g(neighbor, current_distance);
				set_f(neighbor, current_distance + h(world, neighbor));
				open_set.insert(neighbor);
			}
		}
	}
	return {};
}
