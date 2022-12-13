#include "utils.h"
#include <fstream>
#include <iostream>

const std::size_t total_space = 70000000;
const std::size_t required_space = 30000000;
std::size_t total_size = 0;

Node* candidate;

void find_result(Node& node, std::size_t current_space) {
	if(node.type == T_DIR) {
		if(total_space - (current_space - node.size) >= required_space) {
			if(node.size < candidate->size)
				candidate = &node;
		}
		for(auto& child : node.children)
			find_result(child.second, current_space);
	}
}

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	
	Node root = build_filesystem(input);
	display(root, 0);

	candidate = &root;
	find_result(root, root.size);
	std::cout << candidate->size << std::endl;
	return 0;
}

