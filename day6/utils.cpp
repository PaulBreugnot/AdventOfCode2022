#include "utils.h"

std::size_t start_of_sequence(const std::string& buffer, std::size_t sequence_size) {
	for(std::size_t i = sequence_size; i < buffer.size(); i++) {
		bool start = true;
		for(std::size_t j = i-sequence_size; j < i-1; j++) {
			for(std::size_t k = j+1; k < i; k++) {
				if(buffer[k]==buffer[j])
					start = false; 
			}
		}
		if(start)
			return i;
	}
	return buffer.size();
}

std::size_t start_of_packet(const std::string &buffer) {
	return start_of_sequence(buffer, 4);
}

std::size_t start_of_message(const std::string &buffer) {
	return start_of_sequence(buffer, 14);
}
