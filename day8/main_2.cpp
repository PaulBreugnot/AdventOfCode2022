#include "utils.h"
#include <iostream>

int main(int argc, char *argv[])
{
	int max_scenic_score = 0;
	auto input = std::ifstream(argv[1]);
	Trees trees = parse_trees(input);
	for(int i = 0; i < trees.size(); i++) {
		for(int j = 0; j < trees[i].size(); j++) {
			int _scenic_score = scenic_score(trees, {i, j});
			if (_scenic_score > max_scenic_score) {
				max_scenic_score = _scenic_score;
			}
		}
	}
	
	std::cout << max_scenic_score << std::endl;
	return 0;
}
