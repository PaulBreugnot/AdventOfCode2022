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

int Valve::getShortestPath(const Valve *target) const {
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

World::World(std::ifstream& input, int max_time, int agents_count)
	: max_time(max_time), agents_count(agents_count){
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

struct Agent {
	const Valve* target;
};

struct OpenValveEvent {
	int date;
	int agent_id;

	OpenValveEvent(int date, int agent_id)
		: date(date), agent_id(agent_id) {
		}

	bool operator>(const OpenValveEvent& event) const {
		return date > event.date;
	}
};

struct State {
	std::vector<Agent> agents;
	std::priority_queue<
		OpenValveEvent, std::deque<OpenValveEvent>, std::greater<OpenValveEvent>
		> events;
	std::multiset<const Valve*, FlowOrdering> candidate_valves;
	int score;
	int time;

	State(int agent_count, const Valve* current_valve, int score, int time)
		: agents(agent_count), score(score), time(time) {
			for(auto& agent : agents) {
				agents.back().target = current_valve;
				events.push({
						-1, // Trick so that the AA valve is opened even if its
							// useless so that the exploration actually starts at
							// time 0
						0
						});
			}
		}

	int upperBound(int max_time) {
		int upper_bound = score;
		int simulated_time = time;
		auto max_valve = candidate_valves.crbegin();
		while(time < max_time && max_valve != candidate_valves.crend()) {
			// The earliest next date an other valve can start to release pressure
			simulated_time+=2;

			for(std::size_t i = 0; i < agents.size(); i++) {
				if(max_valve == candidate_valves.crend())
					break;
				upper_bound+=(*max_valve)->getFlowRate() * max_time-simulated_time;
				max_valve++;
			}
		}
		return upper_bound;
	}

	bool operator<(const State& state) const {
		return score < state.score;
	}
};

struct StateHash {
	std::size_t operator()(const State& state) const {
		std::ostringstream str;
		str << state.time;
		str << state.agents.back().target->getLabel();
		return std::hash<std::string>()(str.str());
	}
};


int World::solve() const {
	std::priority_queue<State> open_states;
	State start = {agents_count, &valves.find("AA")->second, 0, 0};
	start.candidate_valves = openable_valves;
	open_states.push(start);

	int max_score = 0;
	while(!open_states.empty()) {
		State current_state = open_states.top();
		open_states.pop();

		OpenValveEvent event = current_state.events.top();
		current_state.events.pop();
		
		current_state.time = event.date+1; // Time to reach the valve + time to
										   // open

		const Valve* valve_to_open = current_state.agents[event.agent_id].target;
		current_state.score +=
			(max_time-current_state.time) * valve_to_open->getFlowRate();
		//std::cout << "  Open valve " << open_valve_state.current_valve->getLabel() << ": " << open_valve_state.score << std::endl;
		if(current_state.score > max_score)
			max_score = current_state.score;
		for(auto& valve : current_state.candidate_valves) {
			int shortest_path = valve_to_open->getShortestPath(valve);
			if(current_state.time + shortest_path + 1 < max_time
					&& current_state.upperBound(max_time) > max_score) {
				State next_state = current_state;
				Agent& agent = next_state.agents[event.agent_id];
				agent.target = valve;

				next_state.events.push({
						next_state.time + shortest_path,
						event.agent_id
						});
				next_state.candidate_valves.erase(valve);
				open_states.push(next_state);
			}
		}
	}
	return max_score;
}
