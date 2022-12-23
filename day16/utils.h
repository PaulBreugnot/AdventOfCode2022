#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

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

	public:
		Valve() = default;
		Valve(const std::string& label, int flow_rate);
		void addNeighbor(Valve* valve, int cost);
		void removeNeighbor(const Edge& edge);

		const std::string& getLabel() const;
		const std::unordered_set<Edge, EdgeHash>& getNeighbors() const;
		std::unordered_set<Edge, EdgeHash>& getNeighbors();
		int getFlowRate() const;
};

class World {
	private:
		// Valves go from AA to ZZ so 52 values maximum
		std::unordered_map<std::string, Valve> valves;
		std::unordered_set<std::string> null_valves;
		std::unordered_set<std::string> openable_valves;

		void parse(std::ifstream& input);
	public:
		World(std::ifstream& input);

		void reduceGraph();

		int solve() const;

		const std::unordered_map<std::string, Valve>& getValves() const;
};
