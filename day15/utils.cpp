#include "utils.h"
#include <cmath>
#include <regex>
#include <iostream>
#include <limits>
#include <set>

bool operator==(const Point& p1, const Point& p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

bool operator==(const Range& r1, const Range& r2) {
	return r1.x1 == r2.x1 && r1.x2 == r2.x2;
}

bool operator<(const Range& r1, const Range& r2) {
	if(r1.x1 == r2.x1)
		return r1.x2 < r2.x2;
	return r1.x1 < r2.x1;
}

bool Range::contains(int_type x) const {
	return x >= x1 && x <= x2;
}

std::vector<Range> merge(const Range& r1, const Range& r2) {
	std::vector<Range> d_union;
	if(r1.contains(r2.x1)) {
		if(r1.contains(r2.x2)) {
			// r2 included in r1
			// ####### r1 #########
			//     ####### r2 ###
			d_union.push_back(r1);
		} else if (r2.x2 > r1.x2) {
			// ####### r1 #########
			//               ####### r2 ###
			d_union.push_back({r1.x1, r2.x2});
		}
	} else if (r1.contains(r2.x2)) {
		//       ####### r1 #########
		//    ####### r2 ###
		d_union.push_back({r2.x1, r1.x2});
	} else if (r2.contains(r1.x1)) {
		// r1 included in r2
		//     ####### r1 ###
		// ####### r2 #########
		d_union.push_back(r2);
	} else {
		// Void intersection
		//                       ####### r1 ###
		// ####### r2 #########
		d_union.push_back(r1);
		d_union.push_back(r2);
	}
	return d_union;
}

std::set<Range> merge(const std::set<Range>& ranges) {
	// Result set
	std::set<Range> merged_ranges = ranges;
	auto current = merged_ranges.begin();
	if(current == merged_ranges.end())
		// Void set
		return {};
	auto next = current;
	next++;
	// At each iteration, we build a disjoint union at the beginning of the
	// result set.
	while(next != merged_ranges.end()) {
		// Merges current and next
		auto d_union = merge(*current, *next);
		if(d_union.size() == 2) {
			// The union is already disjoint, so we can move to next item.
			// Since ranges in the set are ordered by their x1 coordinate, it is
			// guaranteed that if current and next are disjoint, then current is
			// necessarilly disjoint with all the next items of the set.
			current++;
			next++;
		} else {
			// Current and next have been merged: replace them with the merged
			// range
			// The merged range is necessarilly inserted at the current position
			auto new_range = merged_ranges.insert(current, d_union[0]); 
			// No need to erase if the new_range is equal to current (i.e. no
			// insertion occured). This corresponds to the case where current
			// was containing next.
			if(new_range != current)
				merged_ranges.erase(current);
			// No need to erase if the new_range is equal to next (i.e. no
			// insertion occured). This corresponds to the case where next
			// was containing current.
			else if(new_range != next)
				merged_ranges.erase(next);

			// The next step restart from the merged range
			//current = new_range;
			current = new_range;
			next = current;
			next++;
		}
	}
	return merged_ranges;
}

const std::regex line_regex {
	"Sensor at x=(-?\\d+), y=(-?\\d+): closest beacon is at x=(-?\\d+), y=(-?\\d+)"
};

int_type Point::distance(const Point& p1, const Point& p2) {
	return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

int_type Range::length() const {
	return std::abs(x2 - x1)+1;
}

int_type Range::length(const std::set<Range>& ranges) {
	std::size_t result = 0;
	for(auto& item : ranges) {
		std::cout << item.x1 << "," << item.x2 << " -> ";
		result+=item.length();
	}
	std::cout << std::endl;
	return result;
}

int_type Range::bounded_length(int min, int max) const {
	return std::abs(
			std::min(std::max(min, x2), max) -
			std::min(std::max(min, x1), max)
			)+1;
}

int_type Range::bounded_length(const std::set<Range>& ranges, int min, int max) {
	std::size_t result = 0;
	for(auto& item : ranges) {
		result+=item.bounded_length(min, max);
	}
	return result;
}

Sensor::Sensor(const Point& location, const Point& beacon)
	: location(location), beacon(beacon), range_size(Point::distance(location, beacon)) {
}

Range Sensor::rangeAtLine(int line) const {
	Range range_at_line;
	range_at_line.x1 = location.x-(range_size - std::abs(location.y - line));
	range_at_line.x2 = location.x+(range_size - std::abs(location.y - line));
	return range_at_line;
}

void World::parse(std::ifstream& input) {
	std::string line;
	std::smatch match;
	for(;std::getline(input, line);) {
		std::regex_match(line, match, line_regex);
		Point beacon = {std::stoi(match[3]), std::stoi(match[4])};
		sensors.emplace_back(
				Point {std::stoi(match[1]), std::stoi(match[2])},
				beacon
				);
		beacons[beacon.y].insert(beacon.x);
	}
}

World::World(std::ifstream& input) {
	parse(input);
	Point origin = {
		std::numeric_limits<int_type>::max(),
		std::numeric_limits<int_type>::max()
	};
	Point extent = {
		std::numeric_limits<int_type>::min(),
		std::numeric_limits<int_type>::min()
	};
	for(const Sensor& sensor : sensors) {
		origin.x = std::min(sensor.getLocation().x-sensor.getRangeSize(), origin.x);
		origin.y = std::min(sensor.getLocation().y-sensor.getRangeSize(), origin.y);
		extent.x = std::max(sensor.getLocation().x+sensor.getRangeSize(), extent.x);
		extent.y = std::max(sensor.getLocation().y+sensor.getRangeSize(), extent.y);
	}
	// Computes the size of the environment, just for fun as we do not need it.
	std::cout << "Origin: " << origin.x << "," << origin.y << std::endl;
	std::cout << "Extent: " << extent.x << "," << extent.y << std::endl;
}

std::set<Range> World::impossible_beacons_ranges(int line) {
	std::set<Range> ranges_at_line;
	for(auto& sensor : sensors) {
		if(std::abs(sensor.getLocation().y-line) <= sensor.getRangeSize()) {
			//std::cout << sensor << std::endl;

			// The sensor can reach the line
			ranges_at_line.insert(sensor.rangeAtLine(line));
		}
	}
	// At this point, we have a set containing a range for each sensor that can
	// reach the line. However, some might overlap. So in order to easily count
	// the location that are covered, we perform a merge of the set of ranges to
	// obtain a disjoint union of ranges that covers exaclty the same area.
	// Notice that since the merge() algorithm is only based on the range
	// bounds, there is never a need to iterate for the actual x positions.
	return merge(ranges_at_line);
}

std::size_t World::impossible_beacons_count(int line) {
	// Since the set returned by impossible_beacons_ranges is a disjoint union,
	// the count of unique positions contained in the range can trivially be
	// obtained with substractions, using the length() methods.
	auto ranges = impossible_beacons_ranges(line);
	std::size_t n = Range::length(ranges);

	// Removes positions where a beacon is already located
	for(auto& item : beacons[line]) {
		for(auto& range : ranges) {
			if(range.contains(item))
				n--;
		}
	}
	return n;
}

std::ostream& operator<<(std::ostream& o, const Sensor& s) {
	o << "S: (" << s.getLocation().x << "," << s.getLocation().y << ") B: ("
		<< s.getBeacon().x << "," << s.getBeacon().y << ") R: "
		<< s.getRangeSize();
	return o;
}
