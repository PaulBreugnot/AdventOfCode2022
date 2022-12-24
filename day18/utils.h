#include <vector>
#include <fstream>
#include <array>

typedef int int_type;

struct Point {
	int_type x;
	int_type y;
	int_type z;

	Point() = default;
	Point(int_type x, int_type y, int_type z)
		: x(x), y(y), z(z) {
		}
	std::array<Point, 6> neighbors() const;

	Point operator+(const Point& p) const;
	bool operator==(const Point& p) const;
};

std::ostream& operator<<(std::ostream& o, const Point& p);

class World {
	typedef std::vector<std::vector<std::vector<int>>> Space;

	private:
		std::vector<Point> points;
		Point origin;
		Point extent;

		Space buildSpace() const;

	public:
		void parse(std::ifstream& input);
		void setOrigin(const Point& origin);
		void setExtent(const Point& extent);

		std::size_t outerSurface(const std::vector<Point>& points) const;
		std::size_t surface(const std::vector<Point>& points) const;

		std::size_t surface() const;
		std::size_t outerSurface() const;
};
