#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	std::size_t visible_trees = 0;
	auto input = std::ifstream(argv[1]);
	Trees trees = parse_trees(input);
	for(int i = 0; i < trees.size(); i++)
		for(int j = 0; j < trees[i].size(); j++)
			for(auto& direction : directions)
				if (is_visible(trees, {i, j}, direction)) {
					visible_trees++;
					break;
				}
	
	std::cout << visible_trees << std::endl;
	return 0;
}
