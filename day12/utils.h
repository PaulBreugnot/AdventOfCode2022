#include <array>
#include <vector>
#include <fstream>

typedef std::array<int, 2> Point;
typedef std::vector<std::vector<int>> Map;
typedef std::vector<Point> Path;

struct World {
	Point start;
	Point target;
	Map map;

	std::vector<Point> neighbors(const Point& p) const;
};

class A_star {
	private:
		class LowF {
			private:
			const A_star& a_star;

			public:
			LowF(const A_star& a_star) : a_star(a_star) {
			}

			bool operator()(const Point& p1, const Point& p2) const;
		};
		std::vector<std::vector<float>> _f;
		std::vector<std::vector<float>> _g;
		std::vector<std::vector<Point>> came_from;
		void init_came_from(const Map& map);
		void init_inf(const Map& map, std::vector<std::vector<float>>& scores);

		float h(const World& world, const Point& p) const;

		float f(const Point& p) const;
		void set_f(const Point& p, float value);

		float g(const Point& p) const;
		void set_g(const Point& p, float value);

		Path build_path(const World& world);
	public:
		Path solve(const World& world);
};

World parse_world(std::ifstream& input);
