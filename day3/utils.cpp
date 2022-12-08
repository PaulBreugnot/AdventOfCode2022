#include "utils.h"

int priority(char item) {
	if (std::islower(item))
		return 1 + (int) item - (int) 'a';
	return 27 + (int) item - (int) 'A';
}
