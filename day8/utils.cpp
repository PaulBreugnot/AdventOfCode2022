#include "utils.h"
#include <string>

Point& operator+=(Point& point, const Direction& direction) {
	point[0] += direction[0];
	point[1] += direction[1];
	return point;
}

Trees parse_trees(std::ifstream& input) {
	Trees trees;
	std::string line;
	for(;std::getline(input, line);) {
		trees.push_back({});
		for(char tree : line)
			trees.back().push_back(std::stoi(std::string {tree}));
	}
	return trees;
}

bool is_visible(const Trees& trees, Point origin, Direction direction) {
	Point current_point = origin;
	bool visible = true;
	current_point+=direction;
	while(visible && current_point[0] >= 0 && current_point[0] < trees.size()
			&& current_point[1] >= 0 && current_point[1] < trees[current_point[0]].size()) {
		if(trees[current_point[0]][current_point[1]] >= trees[origin[0]][origin[1]])
			visible = false;
		current_point += direction;
	}
	return visible;
}

int viewing_distance(const Trees& trees, Point origin, Direction direction) {
	int viewing_distance = 0;
	Point current_point = origin;
	bool visible = true;
	current_point+=direction;
	while(visible && current_point[0] >= 0 && current_point[0] < trees.size()
			&& current_point[1] >= 0 && current_point[1] < trees[current_point[0]].size()) {
		viewing_distance++;
		if(trees[current_point[0]][current_point[1]] >= trees[origin[0]][origin[1]])
			visible = false;
		current_point += direction;
	}
	return viewing_distance;
}

int scenic_score(const Trees& trees, Point origin) {
	int scenic_score = 1;
	for(auto& dir : directions)
		scenic_score*=viewing_distance(trees, origin, dir);
	return scenic_score;
}
