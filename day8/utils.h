#include <fstream>
#include <vector>
#include <array>

typedef std::array<int, 2> Point;
typedef std::array<int, 2> Direction;
typedef std::vector<std::vector<int>> Trees;

const std::array<Direction, 4> directions {{
	{0, 1}, {1, 0}, {-1, 0}, {0, -1}
}};

Point operator+(const Point& point, const Direction& direction);
Trees parse_trees(std::ifstream& input);
bool is_visible(const Trees& trees, Point origin, Direction direction);
int viewing_distance(const Trees& trees, Point origin, Direction direction);
int scenic_score(const Trees& trees, Point origin);
