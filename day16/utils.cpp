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


std::size_t EdgeHash::operator()(const Edge& e) const {
	return std::hash<std::string>()(e.source->getLabel() + e.target->getLabel());
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
	return v1->getFlowRate() > v2->getFlowRate();
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
		valve_targets.emplace(label, match[3]);
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
	std::vector<std::string> valves_to_erase;
	for(auto& valve : valves) {
		if(valve.first != "AA" && valve.second.getFlowRate() == 0) {
			auto edges_to_delete = valve.second.getNeighbors();
			std::cout << "Reducing valve " << valve.first << std::endl;
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
				item.target->removeNeighbor({item.target, &valve.second, 0});
			}
			valves_to_erase.push_back(valve.first);
		}
	}
	for(auto& valve : valves_to_erase)
		valves.erase(valve);
}

struct Agent {
	const Valve* target;
};

struct OpenValveEvent {
	// Date at which the agent will open its target valve (includes the time to
	// go the target valve, and to open it)
	int date;
	// The id of the agent related to this event. The agent can be retrived from
	// agents[agent_id] in the State to which this event belongs.
	int agent_id;

	OpenValveEvent(int date, int agent_id)
		: date(date), agent_id(agent_id) {
		}

	// Used to order events by date
	bool operator>(const OpenValveEvent& event) const {
		return date > event.date;
	}
};

struct State {
	std::vector<Agent> agents;
	std::priority_queue<
		OpenValveEvent, std::deque<OpenValveEvent>, std::greater<OpenValveEvent>
		> events;
	// All the valves that can be opened and are not already targeted by an
	// agent
	std::multiset<const Valve*, FlowOrdering> candidate_valves;
	// Current pressure release
	int score;

	State(int agent_count, const Valve* init_valve, int score)
		: agents(agent_count), score(score) {
			for(int i = 0; i < agents.size(); i++) {
				// All agents start from the init_valve at time 0. In practice,
				// the init valve is AA.
				agents[i].target = init_valve;
				// Time = 0 so the solver behaves as if the agent reach and open
				// the init valve at time 0, what has no effect since the flow
				// rate of AA is 0.
				events.push({0,	i});
			}
		}

	// Quickly estimates the maximum pressure that will be released at the end
	// from the current state. The upper bound must be greater or equal to any
	// value that can actually be obtained, but must be as small as possible to
	// prune large branches of the exploration tree.
	int upperBound(int current_agent, int current_time, int max_time) {
		// Simulates an optimal state from the current state
		State fake_state = *this;
		// In order to have the best estimation possible, we assume that each
		// agent, once he reached its current target, can always reach the best
		// possible valve in only 1 minute, what corresponds to the optimal
		// case.

		// Handles the current agent that just opened a valve, since
		// upperBound() is always called just after an event handling (see
		// solve())
		auto max_valve = fake_state.candidate_valves.begin();
		if(max_valve != fake_state.candidate_valves.end() && current_time+2 < max_time) {
			// Schedules this agent to reach and open the next best valve in
			// only 2 minutes
			fake_state.events.push({current_time+2, current_agent});
			fake_state.agents[current_agent].target = *max_valve;
			fake_state.candidate_valves.erase(max_valve);
		}
		max_valve = fake_state.candidate_valves.begin();
		// Simulated event handling. The current real events are processed, and
		// fake "optimal" events are added until the max_time is reached.
		while(!fake_state.events.empty()) {
			auto event = fake_state.events.top();
			fake_state.events.pop();
			// Estimation of the fake_state score
			fake_state.score+=fake_state.agents[event.agent_id].target->getFlowRate()*(max_time-event.date);
			if(max_valve != fake_state.candidate_valves.end() && current_time+2 < max_time) {
				// Once the agent has reached and opened its real target,
				// schedules it to reach and open the next best valve in only 2
				// minutes
				fake_state.events.push({event.date+2, event.agent_id});
				fake_state.agents[event.agent_id].target = *max_valve;
				fake_state.candidate_valves.erase(max_valve);
				max_valve = fake_state.candidate_valves.begin();
			}
		}
		return fake_state.score;
	}

	bool operator<(const State& state) const {
		return score < state.score;
	}
};

int World::solve() const {
	std::priority_queue<State> open_states;
	State start = {agents_count, &valves.find("AA")->second, 0};
	start.candidate_valves = openable_valves;
	open_states.push(start);

	int num_iterations = 0;
	int max_score = 0;
	while(!open_states.empty()) {
		num_iterations++;
		State current_state = open_states.top();
		open_states.pop();

		OpenValveEvent event = current_state.events.top();
		current_state.events.pop();

		// Reach and open target event processing
		const Valve* valve_to_open = current_state.agents[event.agent_id].target;
		current_state.score +=
			(max_time-event.date) * valve_to_open->getFlowRate();
		if(current_state.score > max_score)
			max_score = current_state.score;
		if(current_state.upperBound(event.agent_id, event.date, max_time) > max_score) {
			// Creates a branch in the exploration tree for all the possible valves
			// the agent that just opened a valve can target
			for(auto& valve : current_state.candidate_valves) {
				// Shortest path to the next valve, that corresponds to the time
				// required to reach it
				int shortest_path = valve_to_open->getShortestPath(valve);
				if(
						// If the time to reach and open the valve is greater or
						// equal to the max_time, it is useless to try to target
						// this valve as it will not have the time to release any
						// pressure
						event.date + shortest_path + 1 < max_time) {
						// If the most optimistic estimation of pressure release 
						//&& current_state.upperBound(event.agent_id, event.date, max_time) > max_score) {
					State next_state = current_state;
					Agent& agent = next_state.agents[event.agent_id];
					agent.target = valve;

					// Planify the event that corresponds to the next valve
					// opening
					next_state.events.push({
							event.date + shortest_path + 1,
							event.agent_id
							});
					next_state.candidate_valves.erase(valve);
					open_states.push(next_state);
				}
			}
			// If there are still events to process in the current state, even
			// if there is no possibility to reach other valves from it, the
			// current state is still opened to simulate it until the last agent
			// reaches its target.
			if(!current_state.events.empty())
				open_states.push(current_state);
		}
	}
	std::cout << "Solved in " << num_iterations << " iterations." << std::endl;
	return max_score;
}
