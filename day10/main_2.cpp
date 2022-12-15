#include "utils.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	std::string line;

	CPU cpu;
	CRT crt;
	for(;std::getline(input, line);) {
		parse_line(line, cpu.current_cycle, cpu.instruction);
		while(!cpu.instruction.done) {
			crt.cycle(cpu.x_register);
			cpu.cycle();
		}
	}
	crt.display();
	return 0;
}
