#include <istream>
#include <vector>
#include <fstream>
#include <array>

typedef std::array<std::size_t, 2> Point;

enum Cell {
	AIR, ROCK, SAND, SOURCE
};

class World {
	protected:
		std::size_t num_sand = 0;
		const Point source {500, 0};
		Point origin;
		Point extent;
		std::vector<std::vector<Cell>> _cells;

		bool in_range(const Point& p) const;

		virtual bool fall() = 0;

		void addRocks(const Point& start, const Point& end);

		void load(std::ifstream& input);
	public:
		Cell& cell(std::size_t x, std::size_t y);
		const std::vector<std::vector<Cell>>& cells() const;

		void simulate();

		std::size_t numSand() const;
};

class SimpleWorld : public World {
	public:
		bool fall() override;

		SimpleWorld(std::ifstream& input);
};

class FullWorld : public World {
	private:
		const std::size_t column_increment = 10;
		void add_left_columns();
		void add_right_columns();
	public:
		bool fall() override;

		FullWorld(std::ifstream& input);
};

std::ostream& operator<<(std::ostream& io, const World& w);
