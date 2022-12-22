#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	std::ifstream input = std::ifstream(argv[1]);
	World world(input);
	for(auto sensor : world.getSensors())
		std::cout << sensor << std::endl;
	std::cout << std::endl;

	int i = 0;
	bool go = true;

	std::size_t max_size = std::stoi(argv[2]);
	while(i < max_size && go) {
		std::set<Range> ranges = world.impossible_beacons_ranges(i);
		std::size_t n = Range::bounded_length(ranges, 0, max_size);

		//std::cout << i << ":" << n << std::endl;
		//for(auto& item : ranges) {
			//std::cout << item.x1 << "," << item.x2 << " - ";
		//}
		//std::cout << std::endl;
		if (n == max_size) {
			int x;
			if ((*ranges.begin()).x1 >= 0)
				// Edge case where the beacon is at position 0 so there is only
				// one Range starting from 1
				x = 0;
			else if ((*ranges.rbegin()).x2 <= max_size)
				// Edge case where the beacon is at position max_size so there is only
				// one Range ending at max_size-1
				x = max_size;
			else
				// Generic case, where there are two ranges with a single free
				// space between them: that is where the beacon is.
				// ##### r1 ###B####### r2 #####
				x = (*ranges.begin()).x2+1;
			std::cout << "The beacon is at (" << x << "," << i << "): frequency " << (unsigned long) x*4000000ll+ (unsigned long int) i << std::endl;
		}
		i++;
	}	
	
	return 0;
}
