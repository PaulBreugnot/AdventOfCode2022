#include "utils.h"
#include <regex>
#include <cmath>

std::map<std::string, Point> c_to_dir {
	{"R", {1, 0}},
	{"L", {-1, 0}},
	{"U", {0, 1}},
	{"D", {0, -1}}
};

Point operator+(const Point& point, const Point& direction) {
	return {{
		point[0] + direction[0],
		point[1] + direction[1]
	}};
}

Point& operator+=(Point& point, const Point& direction) {
	point[0] += direction[0];
	point[1] += direction[1];
	return point;
}

Move parse_move(std::string line) {
	static const std::regex re {"(.*) ([[:digit:]]*)"};
	std::smatch match;
	std::regex_match(line, match, re);

	Move move;
	move.direction = c_to_dir[match[1]];
	move.count = std::stoi(match[2]);
	return move;
}

void move(Point& head_location, Move move) {
	head_location+=move.direction;
}
void follow(Point& head_location, Point& tail_location) {
	if(!in_tail_neighborhood(head_location, tail_location)) {
		Point min_point = tail_location;
		float min_distance = distance(head_location, tail_location);
		for(auto& point : neighborhood(tail_location)) {
			float d = distance(head_location, point);
			if (d < min_distance) {
				min_distance = d;
				min_point = point;
			}
		}
		tail_location = min_point;
	}
}

float distance(const Point& p1, const Point& p2) {
	return std::sqrt(std::pow(p2[0]-p1[0],2)+std::pow(p2[1]-p1[1],2));
}

std::vector<Point> neighborhood(Point point) {
	return {
		point,
		point + Point {0, 1},
		point + Point {1, 0},
		point + Point {0, -1},
		point + Point {-1, 0},
		point + Point {1, 1},
		point + Point {-1, 1},
		point + Point {1, -1},
		point + Point {-1, -1}
	};
}

bool in_tail_neighborhood(Point &head_location, Point &tail_location) {
	auto n = neighborhood(tail_location);
	return std::find(n.begin(), n.end(), head_location) != n.end();
}
