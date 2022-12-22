#include <array>
#include <vector>
#include <set>
#include <fstream>
#include <unordered_map>

typedef int int_type;

/**
 * A 2D point in space.
 */
struct Point {
	int_type x;
	int_type y;

	Point() = default;
	Point(int_type x, int_type y)
		: x(x), y(y) {
		}

	/**
	 * Manhattan distance between two points.
	 */
	static int_type distance(const Point& p1, const Point& p2);
};

/**
 * Represents an horizontal range [x1, x2] on a given line. x1 and x2 are
 * **included**.
 *
 * A range r at line y can be represented as follows:
 *        x               x
 *        1               2
 * y|     ###### r ########              |
 */
struct Range {
	int_type x1;
	int_type x2;

	Range() = default;
	Range(int_type x1, int_type x2)
		: x1(x1), x2(x2) {
		}

	/**
	 * Returns true iff x is contained in the range.
	 */
	bool contains(int_type x) const;

	/**
	 * Returns the length of the range, i.e. the total count of points in the
	 * range [x1, x2].
	 */
	int_type length() const;
	/**
	 * Returns the length of the range croped to [min, max].
	 */
	int_type bounded_length(int min, int max) const;

	/**
	 * Returns the sum of the length of ranges.
	 */
	static int_type length(const std::set<Range>& ranges);
	/**
	 * Returns the sum of the length of ranges croped to [min, max].
	 */
	static int_type bounded_length(const std::set<Range>& ranges, int min, int max);
};

/**
 * Two points are equal if their x and y coordinates are equal.
 */
bool operator==(const Point& p1, const Point& p2);

/**
 * Two ranges are equal if their x1 and x2 bounds are equal.
 */
bool operator==(const Range& r1, const Range& r2);

/**
 * Orders ranges according to the x coordinate only.
 */
bool operator<(const Range& r1, const Range& r2);

/**
 * Merges the two ranges, if possible.
 *
 * If they is no intersection between the two ranges, a vector containing r1 and
 * r2 is returned.
 *
 * Else, a single range stricly containing r1 and r2 is returned.
 *
 * # Examples
 * Note: r1 and r2 are by definiton considered to be in the same line.
 *
 * r1:     |        ###### r1 ######                        |
 * r2:     |                           #### r2 ####         |
 * result: {        ################   ############         } (size = 2)
 *
 * r1:     |        ###### r1 ######                        |
 * r2:     |                  #### r2 ####                  |
 * result: {        ######################                  } (size = 1)
 *
 * r1:     |                 ###### r1 ######               |
 * r2:     |            #### r2 ####                        |
 * result: {            #####################               } (size = 1)
 *
 * r1:     |                 ###### r1 ######               |
 * r2:     |                   #### r2 ####                 |
 * result: {                 ################               } (size = 1)
 */
std::vector<Range> merge(const Range& r1, const Range& r2);

/**
 * Returns a set that exactly covers the same area as the input ranges, but the
 * returned set is consituted by disjoint ranges only.
 *
 * The result is obtained by merging all the ranges that it is possible to merge
 * in the input ranges.
 *
 * # Examples
 * Note: ranges in the input set are ordered by their x1 bound (see
 * the definition of operator<(const Range&, const Range&)).
 *
 * r1:     |        ###### r1 ######                        |
 * r2:     |                   #### r3 ####                 |
 * r3:     |                           #### r2 ####         |
 * result: {        #############################         } (size = 1)
 *
 * r1:     |        ###### r1 ######                        |
 * r2:     |              #### r3 ####                      |
 * r3:     |                                 ### r2 ####    |
 * result: {        ##################       ###########    } (size = 2)
 *
 * r1:     |        ###### r1 ######                        |
 * r2:     |              #### r3 ########################  |
 * r3:     |                                 ### r2 ####    |
 * result: {        ######################################  } (size = 2)
 */
std::set<Range> merge(const std::set<Range>& ranges);

/**
 * Class representing a Sensor.
 *
 * A Sensor at (x,y) with a range size of 2 can be represented as follows:
 *
 *         x
 *
 *         #
 *        ###
 * y     ##S##
 *        ###
 *         #
 */
class Sensor {
	private:
		Point location;
		Point beacon;
		int range_size;
	public:
		/**
		 * Location of the sensor.
		 */
		const Point& getLocation() const {
			return location;
		}
		/**
		 * Location of the nearest beacon.
		 */
		const Point& getBeacon() const {
			return beacon;
		}
		/**
		 * Size of the range of the sensor.
		 */
		int getRangeSize() const {
			return range_size;
		}

		/**
		 * Returns the ranges that the sensor covers at line.
		 *
		 * The method assumes that the sensor can effectively reach the line,
		 * i.e. |line-location.y| <= range_size, as we cannot return a void
		 * range.
		 *
		 * For example, for a sensor at (x,y) with range 4:
		 *
		 *         x
		 *         .
		 *        ...
		 *       .....
		 *      .......
		 * y   ....S....
		 *      ...|...
		 * l     #####
		 *        ...
		 *         .
		 *       x   x
		 *       1   2
		 *
		 * x1 and x2 can trivially be computed from the range size of the
		 * sensor, such as (x-x1)+(y-l) = range_size (Manhattan distance):
		 * - x1 = x-(range_size-|l-h|)
		 * - x2 = x+(range_size-|l-h|)
		 */
		Range rangeAtLine(int line) const;

		Sensor(const Point& location, const Point& beacon);
};

/**
 * Main class containing all problem data. It is only required to store the list
 * of Sensors and Beacons.
 */
class World {
	private:
		std::vector<Sensor> sensors;
		std::unordered_map<int, std::set<int>> beacons;

		void parse(std::ifstream& input); 

	public:
		/**
		 * List of sensors.
		 */
		const std::vector<Sensor> getSensors() const {
			return sensors;
		}

		/**
		 * Count the position where a beacon cannot be located at the provided
		 * line, according to the coverage of the sensors.
		 *
		 * Complexity: O(S) where S=sensors.size()
		 */
		std::size_t impossible_beacons_count(int line);
		/**
		 * Returns the disjoint set of ranges that covers the location where a
		 * beacon cannot be location at the given line, according to the
		 * coverage of the sensors.
		 *
		 * Complexity: O(S) where S=sensors.size()
		 */
		std::set<Range> impossible_beacons_ranges(int line);

		/**
		 * Parse input and initializes the list of sensors.
		 */
		World(std::ifstream& input);

};

std::ostream& operator<<(std::ostream& o, const Sensor& s);
