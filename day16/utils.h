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
	std::size_t operator()(const Edge& e) const {
		return std::hash<Valve*>()(e.target);
	}
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

struct FlowOrdering {
	bool operator()(const Valve* v1, const Valve* v2) const;
};

class World {
	private:
		int max_time;
		int agents_count;
		// Valves go from AA to ZZ so 52 values maximum
		std::unordered_map<std::string, Valve> valves;
		std::unordered_set<Valve*> null_valves;
		std::multiset<const Valve*, FlowOrdering> openable_valves;
		std::size_t num_edges;

		void parse(std::ifstream& input);

		void allPairsShortestPaths();

	public:
		World(std::ifstream& input, int max_time, int agents_count);

		void reduceGraph();

		int solve() const;

		const std::unordered_map<std::string, Valve>& getValves() const;
};
