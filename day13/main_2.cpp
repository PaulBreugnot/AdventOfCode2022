#include "utils.h"
#include <fstream>
#include <set>
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	std::multiset<std::pair<std::string, Packet*>, PacketComparator> packet_set;
	packet_set.insert({"[[2]]", parse_packet("[[2]]")});
	packet_set.insert({"[[6]]", parse_packet("[[6]]")});

	std::string line;
	for(;std::getline(input, line);) {
		if(line.size() > 0)
			packet_set.insert({line, parse_packet(line)});
	}
	int begin = 0;
	int end = 0;
	int i = 1;
	for(auto item : packet_set) {
		if(item.first == "[[2]]")
			begin = i;
		else if (item.first == "[[6]]")
			end = i;
		i++;
		std::cout << item.first << std::endl;
	}
	std::cout << begin * end << std::endl;
	
	return 0;
}
