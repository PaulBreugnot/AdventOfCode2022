#include <fstream>
#include <unordered_map>
#include <iostream>

#include "score.h"

std::unordered_map<char, Choice> convert_choice {
	{'A', ROCK},
	{'B', PAPER},
	{'C', SCISSORS}
};

enum Result {
	WIN, LOSE, DRAW
};
std::unordered_map<char, Result> convert_result {
	{'X', LOSE},
	{'Y', DRAW},
	{'Z', WIN}
};

int main(int argc, char *argv[])
{
	int your_score = 0;
	auto input = std::ifstream(argv[1]);
	for(std::string line ; std::getline(input, line);) {
		std::array<Choice, 2> round;
		round[0] = convert_choice[line[0]];

		Result result = convert_result[line[2]];
		switch(result) {
			case DRAW:
				round[1] = round[0];
				break;
			case LOSE:
				switch(round[0]) {
					case ROCK:
						round[1] = SCISSORS;
						break;
					case PAPER:
						round[1] = ROCK;
						break;
					case SCISSORS:
						round[1] = PAPER;
				}
				break;
			case WIN:
				switch(round[0]) {
					case ROCK:
						round[1] = PAPER;
						break;
					case PAPER:
						round[1] = SCISSORS;
						break;
					case SCISSORS:
						round[1] = ROCK;
				}
		}
		your_score += score(round);
	}
	std::cout << "Score: " << your_score << std::endl;
	return 0;
}
