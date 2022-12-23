#include "utils.h"
#include <fstream>
#include <iostream>

void dot_output(const World& world, std::string output_file) {
	auto output = std::ofstream(output_file);
	output << "digraph {" << std::endl;
	for(auto& valve : world.getValves()) {
		if(valve.second.getFlowRate() == 0)
			output << valve.second.getLabel() << "[color = red];" << std::endl;
		for(auto& neighbor : valve.second.getNeighbors())
			output << valve.second.getLabel() << " -> " << neighbor.target->getLabel() << ";" << std::endl;
	}
	output << "}" << std::endl;
}

int main(int argc, char *argv[])
{
	auto input = std::ifstream(argv[1]);

	World world(input);
	dot_output(world, argv[2]);

	world.reduceGraph();

	dot_output(world, "reduced_" + std::string(argv[2]));
	
	return 0;
}
