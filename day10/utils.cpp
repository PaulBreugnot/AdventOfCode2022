#include "utils.h"
#include <regex>
#include <iostream>

std::regex noop_regex {"noop"};
std::regex addx_regex {"addx (-*[[:digit:]]+)"};

void parse_line(std::string line, const int& cycle, Instruction& instruction) {
	std::smatch match;
	instruction.begin_cycle = cycle;
	instruction.done = false;
	if(std::regex_match(line, match, noop_regex)) {
		instruction.command = NOOP;
	} else if (std::regex_match(line, match, addx_regex)) {
		instruction.command = ADDX;
		instruction.value = std::stoi(match[1]);
	}
}

void CPU::cycle() {
	current_cycle++;
	switch(instruction.command) {
		case ADDX:
			if(current_cycle-instruction.begin_cycle == 2) {
				x_register+=instruction.value;
				instruction.done = true;
			} else {
				instruction.done = false;
			}
			break;
		case NOOP:
			instruction.done = true;
	}
}

void CRT::cycle(const int& x_register) {
	if(std::abs(x_register-(position%40)) <= 1)
		screen[position/40][position%40] = '#';
	else
		screen[position/40][position%40] = '.';
	position++;
}

void CRT::display() {
	for(auto& line : screen) {
		for(char column : line)
			std::cout << column;
		std::cout << std::endl;
	}
}
