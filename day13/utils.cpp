#include "utils.h"
#include <regex>
#include <iostream>

int compare(const Packet& left, const Packet& right) {
	if(const auto* left_int = dynamic_cast<const IntPacket*>(&left)) {
		if(const auto* right_int = dynamic_cast<const IntPacket*>(&right)) {
			return compare(*left_int, *right_int);
		} else if(const auto* right_vec = dynamic_cast<const VectorPacket*>(&right)) {
			return compare(*left_int, *right_vec);
		}
	} else if(const auto* left_vec = dynamic_cast<const VectorPacket*>(&left)) {
		if(const auto* right_int = dynamic_cast<const IntPacket*>(&right)) {
			return compare(*left_vec, *right_int);
		} else if(const auto* right_vec = dynamic_cast<const VectorPacket*>(&right)) {
			return compare(*left_vec, *right_vec);
		}
	}
	return 0;
}

int compare(const IntPacket& left, const IntPacket& right) {
	if(left.value < right.value)
		return -1;
	if (left.value > right.value)
		return 1;
	return 0;
}

int compare(const IntPacket& left, const VectorPacket& right) {
	VectorPacket left_vec;
	left_vec.packets.push_back(new IntPacket(left.value));
	return compare(left_vec, right);
}

int compare(const VectorPacket& left, const IntPacket& right) {
	VectorPacket right_vec;
	right_vec.packets.push_back(new IntPacket(right.value));
	return compare(left, right_vec);
}

int compare(const VectorPacket& left, const VectorPacket& right) {
	int result = 0;
	auto left_it = left.packets.begin();
	auto right_it = right.packets.begin();
	while(result == 0 && left_it != left.packets.end() && right_it != right.packets.end()) {
		result = compare(**left_it, **right_it);
		left_it++;
		right_it++;
	}
	if(result==0 && left_it == left.packets.end() && right_it != right.packets.end())
		return -1;
	else if(result==0 && right_it == right.packets.end() && left_it != left.packets.end())
		return 1;
	return result;
}

std::vector<std::string> parse_items(std::string list_str) {
	std::vector<std::string> items;
	int begin_item_index = 0;
	int current_item_index = 0;
	int level = 0;
	bool close = false;
	while(!close && current_item_index!=list_str.size()) {
		if(list_str[current_item_index] == '[') {
			level++;
		} else if(list_str[current_item_index] == ']') {
			level--;
		} else if(list_str[current_item_index] == ',' && level == 0) {
			std::string item = list_str.substr(begin_item_index, current_item_index-begin_item_index); 
			// Removes useless spaces
			std::smatch match;
			std::regex_match(item, match, std::regex {"\\s*(.*)\\s*"});
			//std::cout << "item: " << match[1] << std::endl;
			items.push_back(match[1]);
			begin_item_index = current_item_index+1;
		}
		current_item_index++;
	}
	if(current_item_index != begin_item_index) {
		// last item
		std::string item = list_str.substr(begin_item_index, current_item_index-begin_item_index); 
		// Removes useless spaces
		std::smatch match;
		std::regex_match(item, match, std::regex {"\\s*(.*)\\s*"});
		//std::cout << "item: " << match[1] << std::endl;
		items.push_back(match[1]);
		begin_item_index = current_item_index+1;
	}

	return items;
}

Packet* parse_packet(std::string str) {
	std::smatch match;
	if(std::regex_match(str, match, std::regex {"(\\d+)"})) {
		//std::cout << "int: " << match[1] << std::endl;
		return new IntPacket(std::stoi(match[1]));
	} else if(std::regex_match(str, match, std::regex {"\\[(.*)\\]"})) {
		//std::cout << "vec: " << match[1] << std::endl;
		const std::string content = match[1];
		VectorPacket* vec_packet = new VectorPacket;
		//std::cout << content << std::endl;
		for(auto item : parse_items(content)) {
			vec_packet->packets.push_back(parse_packet(item));
		}
		return vec_packet;
	} else {
		std::cout << "Parse error: " << str << std::endl;
	}
	return nullptr;
}
