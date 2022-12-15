#include <string>
#include <array>

enum Command {
	NOOP, ADDX
};

struct Instruction {
	Command command;
	int begin_cycle;
	int value;
	bool done;
};

class CPU {
	public:
		int current_cycle = 0;
		int x_register = 1;
		Instruction instruction;
		void cycle();
};

class CRT {
	public:
		int position = 0;
		std::array<char, 3> sprite {{'#', '#', '#'}};
		std::array<std::array<char, 40>, 6> screen;

		void cycle(const int& x_register);
		void display();
};

void parse_line(std::string line, const int& cycle, Instruction& instruction);
