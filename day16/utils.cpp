#include "utils.h"
#include <regex>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <queue>
#include <limits>

const std::regex valve_regex {
	"Valve (\\w+) has flow rate=(\\d+); tunnels? leads? to valves? (.*)"
};

bool Edge::operator==(const Edge& e) const {
	return source == e.source && target == e.target;
}

Valve::Valve(const std::string& label, int flow_rate)
	: label(label), flow_rate(flow_rate) {
}

void Valve::addNeighbor(Valve *valve, int cost) {
	neighbors.insert({this, valve, cost});
}
void Valve::removeNeighbor(const Edge& e) {
	neighbors.erase(e);
}

const std::string& Valve::getLabel() const {
	return label;
}

const std::unordered_set<Edge, EdgeHash>& Valve::getNeighbors() const {
	return neighbors;
}

std::unordered_set<Edge, EdgeHash>& Valve::getNeighbors() {
	return neighbors;
}

int Valve::getFlowRate() const {
	return flow_rate;
}

void Valve::setShortestPath(const Valve *target, int path_length) {
	shortest_paths[target] = path_length;
}

int Valve::getShortestPath(const Valve *target) {
	return shortest_paths.find(target)->second;
}

const std::unordered_map<std::string, Valve>& World::getValves() const {
	return valves;
}

bool FlowOrdering::operator()(const Valve *v1, const Valve *v2) const {
	return v1->getFlowRate() < v2->getFlowRate();
}

void World::allPairsShortestPaths() {
	for(auto& valve : valves) {
		for(auto& target : valves) {
			if(&valve.second == &target.second)
				valve.second.setShortestPath(&target.second, 0);
			else
				valve.second.setShortestPath(&target.second, std::numeric_limits<int>::max());
		}
		// k=0
		for(auto& neighbor : valve.second.getNeighbors())
			valve.second.setShortestPath(neighbor.target, neighbor.cost);
	}
	for(auto& valve : valves) {
		for(auto& source : valves) {
			for(auto& target : valves) {
				int cost1 = source.second.getShortestPath(&valve.second);
				int cost2 = valve.second.getShortestPath(&target.second);
				if(cost1 != std::numeric_limits<int>::max()
						&& cost2 != std::numeric_limits<int>::max()) {
					int cost = cost1+cost2;
					if(source.second.getShortestPath(&target.second) > cost) {
						source.second.setShortestPath(&target.second, cost);
					}
				}
			}
		}
	}
#ifdef LOG
	for(auto& valve : valves) {
		std::cout << "Shortest paths from valve " << valve.first << std::endl;
		for(auto& target : valves) {
			std::cout << "  to " << target.first << ": " << valve.second.getShortestPath(&target.second) << std::endl;
		}
	}
#endif
}

void World::parse(std::ifstream& input) {
	std::unordered_map<std::string, const std::string> valve_targets;
	std::string line;
	std::smatch match;
	for(;std::getline(input, line);) {
		std::regex_match(line, match, valve_regex);
		std::string label = match[1];
		int flow_rate = std::stoi(match[2]);
		valves[label] = {label, flow_rate};
		if(flow_rate > 0)
			openable_valves.insert(&valves[label]);
		else if(label != "AA")
			null_valves.insert(&valves[label]);
		valve_targets.emplace(label, match[3]);
		num_edges++;
	}
	for(auto& item : valve_targets) {
		auto begin = item.second.begin();
		auto end = item.second.end();
		while(std::regex_search(begin, end, match, std::regex {"(\\w+)"})) {
			std::string neighor_label = match[1];
			valves[item.first].addNeighbor(&valves[neighor_label], 1);

			begin = match[0].second;
		}
	}
	for(auto& item : valves) {
		std::cout << "Valve " << item.first
			<< " (flow " << item.second.getFlowRate() << ") : ";
		for(auto& edge : item.second.getNeighbors())
			std::cout << edge.target->getLabel() << " ";
		std::cout << std::endl;

	}
}

World::World(std::ifstream& input) {
	parse(input);
	reduceGraph();
	allPairsShortestPaths();
}

void World::reduceGraph() {
	for(Valve* null_valve : null_valves) {
		auto edges_to_delete = null_valve->getNeighbors();
		std::cout << "Reducing valve " << null_valve->getLabel() << std::endl;
		for(auto& edge1 : edges_to_delete) {
			for(auto& edge2 : edges_to_delete) {
				int cost = edge1.cost+edge2.cost;
				if(edge1.target != edge2.target) {
					std::cout << "  Link " << edge1.target->getLabel() << " to " << edge2.target->getLabel() << std::endl;
					edge1.target->addNeighbor(edge2.target, cost);
				}
			}
		}
		for(auto& item : edges_to_delete) {
			item.target->removeNeighbor({item.target, null_valve, 0});
		}
		valves.erase(null_valve->getLabel());
	}
}

struct State {
	std::unordered_set<const Valve*> open_valves;
	std::set<const Valve*, FlowOrdering> candidate_valves;
	const Valve* current_valve;
	int score;
	int time;

	State(const Valve* current_valve, int score, int time)
		: current_valve(current_valve), score(score), time(time) {
		}

	void open() {
		open_valves.insert(current_valve);
		candidate_valves.erase(current_valve);
	}

	bool currentValveOpen() const {
		return open_valves.find(current_valve) != open_valves.end();
	}

	int upperBound() {
		int upper_bound = score;
		int simulated_time = time;
		auto max_valve = candidate_valves.crbegin();
		while(time < 30 && max_valve != candidate_valves.crend()) {
			if(*max_valve != current_valve)
				// The next date an other valve can start to release pressure
				simulated_time+=2;
			else if(!currentValveOpen())
				// Time to open the current valve
				simulated_time+=1;
			else
				// The current valve is alread opened
				simulated_time+=2;

			upper_bound+=(*max_valve)->getFlowRate() * 30-simulated_time;
			max_valve++;
		}
		return upper_bound;
	}

	bool operator==(const State& state) const {
		return (current_valve->getLabel() == state.current_valve->getLabel())
			&& open_valves == state.open_valves;
	}
	bool operator<(const State& state) const {
		return score < state.score;
	}
};

struct StateHash {
	std::size_t operator()(const State& state) const {
		std::ostringstream str;
		str << state.time;
		str << state.current_valve->getLabel();
		return std::hash<std::string>()(str.str());
	}
};


int World::solve() const {
	std::unordered_set<State, StateHash> explored_states;
	std::priority_queue<State> open_states;
	State start = {&valves.find("AA")->second, 0, 0};
	start.candidate_valves = openable_valves;
	open_states.push(start);

	int max_score = 0;
	while(!open_states.empty()) {
		State current_state = open_states.top();
		open_states.pop();
		//std::cout << "Explore valve " << current_state.current_valve->getLabel() << " at time " << current_state.time << std::endl;
		explored_states.insert(current_state);
		if(current_state.time < 30) {
			if(current_state.current_valve->getFlowRate() > 0
					&& current_state.open_valves.find(current_state.current_valve) == current_state.open_valves.end()) {
				State open_valve_state {current_state};
				open_valve_state.time = current_state.time+1;
				open_valve_state.score +=
					(30-open_valve_state.time) * open_valve_state.current_valve->getFlowRate();
				open_valve_state.open();
				//std::cout << "  Open valve " << open_valve_state.current_valve->getLabel() << ": " << open_valve_state.score << std::endl;
				if(open_valve_state.score > max_score)
					max_score = open_valve_state.score;

				open_states.push(open_valve_state);
			}
			for(const Edge& edge : current_state.current_valve->getNeighbors()) {
				State next_valve_state {current_state};
				next_valve_state.current_valve = edge.target;
				next_valve_state.score = current_state.score; // No score
															  // increment
				next_valve_state.time = current_state.time + edge.cost;
				// Open nothing, go to next valve
				auto explored_state = explored_states.find(next_valve_state);
				if(explored_state == explored_states.end()) {
					if(next_valve_state.upperBound() > max_score)
						open_states.push(next_valve_state);
				} else {
					if(next_valve_state.score > explored_state->score) {
						if(next_valve_state.upperBound() > max_score) {
							// Removes the old score
							explored_states.erase(explored_state);
							open_states.push(next_valve_state);
						}
					}
				}
			}

		}
	}
	return max_score;
}
