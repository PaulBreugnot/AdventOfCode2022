#include <vector>
#include <array>
#include <fstream>
#include <cassert>

#ifdef NDEBUG
#define LOG
#endif

struct Point {
	int x;
	int y;

	Point() = default;
	Point(int x, int y) : x(x), y(y) {
	}
};

struct Rock {
	const std::vector<std::vector<int>> shape;
	const int width;
	const int height;

	Rock(
			const std::vector<std::vector<int>>& shape,
			const int& width, const int& height)
		: shape(shape), width(width), height(height) {
		}
};

extern const std::array<Rock, 5> rocks;
	
enum Jet : int {
	LEFT = -1,
	RIGHT = +1
};

class FallingRock {
	private:
		// Location of the bottom left point of the rock
		Point _location;
	public:
		const Rock* rock;

		FallingRock(const Rock* rock, const Point& location)
			: rock(rock), _location(location) {
			};

		const Point& location() const {
			return _location;
		}

		Point& location() {
			return _location;
		}
};

class World {
	friend std::ostream& operator<<(std::ostream&, const World&);
	public:
	static constexpr std::size_t CAVE_WIDTH = 7;
	private:
		FallingRock falling_rock;
		std::vector<Jet> jet_sequence;
		std::vector<std::array<int, CAVE_WIDTH>> cave;
		void parse(std::ifstream& input);
		int rock_count;
		int time;
		int tower_top;

		Jet currentJet() const;
		void spawnRock();

		bool fallRock();
		void pushRock();
	public:
		int rockCount() const;
		int towerTop() const;
		void step();
		void drop(std::size_t rock_count);
		World(std::ifstream& input);

};

std::ostream& operator<<(std::ostream& o, const World& world);

