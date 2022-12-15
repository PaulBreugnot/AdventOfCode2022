#include "utils.h"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	std::string line;

	int probe = 0;
	int probe_begin = 20;
	int probe_period = 40;
	CPU cpu;
	for(;std::getline(input, line);) {
		parse_line(line, cpu.current_cycle, cpu.instruction);
		while(!cpu.instruction.done) {
			if(((cpu.current_cycle+1)-20)%40 == 0)
				probe+=(cpu.current_cycle+1)*cpu.x_register;
			cpu.cycle();
		}
	}
	std::cout << probe << std::endl;
	return 0;
}
