#include <fstream>
#include <iostream>
#include <array>

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);
	std::array<unsigned int, 3> max_calories {0, 0, 0};
	unsigned int calories = 0;
	for(std::string line ; std::getline(input, line);) {
		if(line.size() > 0) {
			calories += std::stoi(line);
		} else {
			if (calories > max_calories[0]) {
				max_calories[0] = calories;
				for(int i = 0; i < 2; i++) {
					if(max_calories[i] > max_calories[i+1])
						std::swap(max_calories[i], max_calories[i+1]);
				}
			}
			calories = 0;
		}
			
	}
	std::cout << "Max calories: [";
	std::cout << max_calories[0] << ", ";
	std::cout << max_calories[1] << ", ";
	std::cout << max_calories[2] << "]" << std::endl;
	std::cout << "Sum: " << max_calories[0] + max_calories[1] + max_calories[2] << std::endl;
	return 0;
}
