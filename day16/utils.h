#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>

//#define LOG

class Valve;

struct Edge {
	Valve* source;
	Valve* target;
	int cost;

	Edge(Valve* source, Valve* target, int cost)
		: source(source), target(target), cost(cost) {
		}

	bool operator==(const Edge& e) const;
};

struct EdgeHash {
	std::size_t operator()(const Edge& e) const;
};

class Valve {
	private:
		std::string label;
		int flow_rate;
		std::unordered_set<Edge, EdgeHash> neighbors;
		std::unordered_map<const Valve*, int> shortest_paths;

	public:
		Valve() = default;
		Valve(const std::string& label, int flow_rate);
		void addNeighbor(Valve* valve, int cost);
		void removeNeighbor(const Edge& edge);

		const std::string& getLabel() const;
		const std::unordered_set<Edge, EdgeHash>& getNeighbors() const;
		std::unordered_set<Edge, EdgeHash>& getNeighbors();

		int getFlowRate() const;
		void setShortestPath(const Valve* target, int path_length);
		int getShortestPath(const Valve* target) const;
};

/**
 * Orders valves by their flow in descending order so that the valves with the
 * max flow are at the beginning of Valve ordered set built with this
 * comparator.
 */
struct FlowOrdering {
	bool operator()(const Valve* v1, const Valve* v2) const;
};

class World {
	private:
		int max_time;
		int agents_count;
		// All the valves, without null valves if reduceGraph() has been called
		// but still including AA
		std::unordered_map<std::string, Valve> valves;
		// All the valves with a flow rate > 0, ordered by flow rate such that
		// the valve with the maximum flow rate is at openable_valves.begin()
		std::multiset<const Valve*, FlowOrdering> openable_valves;

		void parse(std::ifstream& input);

		// Uses the Floyd-Warshall algorithm to build the shortest_paths of each
		// Valve
		void allPairsShortestPaths();

	public:
		World(std::ifstream& input, int max_time, int agents_count);

		// Removes all the valve with a flow rate of 0, expect AA. The cost of
		// new edges is consistent with the original graph.
		void reduceGraph();

		int solve() const;

		const std::unordered_map<std::string, Valve>& getValves() const;
};
