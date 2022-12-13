#include "utils.h"
#include <fstream>
#include <iostream>

std::size_t total_size = 0;

std::size_t find_result(Node& node) {
	if(node.type == T_DIR) {
		if(node.size < 100000)
			total_size += node.size;
		for(auto& item : node.children) {
			find_result(item.second);
		}
	}
	return node.size;
}

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	
	Node root = build_filesystem(input);
	display(root, 0);
	find_result(root);
	std::cout << total_size << std::endl;
	return 0;
}

