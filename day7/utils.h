#include <map>
#include <string>

enum Type {
	T_DIR, T_FILE
};

struct Node {
	Type type;
	std::string name;
	std::size_t size;
	Node* parent;
	std::map<std::string, Node> children;
};

void explore(Node& node, std::string line, std::ifstream& input);
std::size_t size(Node& node);

Node build_filesystem(std::ifstream& input);

void display(const Node& node, int level);
