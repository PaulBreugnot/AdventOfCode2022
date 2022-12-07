#include <fstream>
#include <iostream>
#include <unordered_map>

#include "score.h"

std::unordered_map<char, Choice> convert {
	{'A', ROCK},
	{'B', PAPER},
	{'C', SCISSORS},
	{'X', ROCK},
	{'Y', PAPER},
	{'Z', SCISSORS}
};

int main(int argc, char *argv[])
{
	int your_score = 0;
	auto input = std::ifstream(argv[1]);
	for(std::string line ; std::getline(input, line);) {
		std::array<Choice, 2> round {
			convert[line[0]], convert[line[2]]
		};
		your_score += score(round);
	}
	std::cout << "Score: " << your_score << std::endl;
	return 0;
}
