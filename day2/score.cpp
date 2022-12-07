#include "score.h"

int score(std::array<Choice, 2> round) {
	int score = 0;

	switch(round[1]) {
		case ROCK:
			score+=1;
			switch(round[0]) {
				case ROCK:
					score+=3;
					break;
				case PAPER:
					break;
				case SCISSORS:
					score+=6;
			}
			break;
		case PAPER:
			score+=2;
			switch(round[0]) {
				case ROCK:
					score+=6;
					break;
				case PAPER:
					score+=3;
					break;
				case SCISSORS:
					break;
			}
			break;
		case SCISSORS:
			score+=3;
			switch(round[0]) {
				case ROCK:
					break;
				case PAPER:
					score+=6;
					break;
				case SCISSORS:
					score+=3;
					break;
			}
			break;
	}

	return score;
}
