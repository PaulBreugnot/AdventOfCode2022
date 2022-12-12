#include <string>

std::size_t start_sequence(const std::string& buffer, std::size_t sequence_size);
std::size_t start_of_packet(const std::string& buffer);
std::size_t start_of_message(const std::string& buffer);
