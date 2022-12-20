#include <istream>
#include <vector>
#include <fstream>
#include <array>

typedef std::array<std::size_t, 2> Point;

enum Cell {
	AIR, ROCK, SAND, SOURCE
};

class World {
	private:
		std::size_t num_sand = 0;
		const Point source {500, 0};
		Point origin;
		Point extent;
		std::vector<std::vector<Cell>> _cells;

		bool in_range(const Point& p) const;
		bool fall();

	public:
		Cell& cell(std::size_t x, std::size_t y);
		const std::vector<std::vector<Cell>>& cells() const;

		void load(std::ifstream& input);

		void simulate();

		std::size_t numSand() const;
};

std::ostream& operator<<(std::ostream& io, const World& w);
