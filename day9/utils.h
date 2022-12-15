#include <array>
#include <string>
#include <vector>

typedef std::array<int, 2> Point;

struct Move {
	Point direction;
	int count;
};

Point operator+(const Point& point, const Point& direction);
Point& operator+=(Point& point, const Point& direction);
Move parse_move(std::string line);
void move(Point& head_location, Move move);
void follow(Point& head_location, Point& tail_location);
std::vector<Point> neighborhood(Point point);
bool in_tail_neighborhood(Point& head_location, Point& tail_location);
float distance(const Point& p1, const Point& p2);
