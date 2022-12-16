#include <deque>
#include <functional>
#include <fstream>
#include <vector>

struct Operator {
	std::function<int(const int&)> left;
	std::function<int(const int&)> right;
};

struct Plus : public Operator{
	int operator()(const int& old) {
		return left(old) + right(old);
	}
};

struct Multiplies : public Operator {
	int operator()(const int& old) {
		return left(old) * right(old);
	}
};

struct DivisibleBy {
	int i;
	int operator()(const int& value) {
		return value % i == 0;
	}
};

struct Old {
	int operator()(const int& old) {
		return old;
	}
};

struct Int {
	int value;
	int operator()(const int& old) {
		return value;
	}
};


class Monkey {
	public:
		int test_count = 0;
		int id;
		std::deque<int> items;

		std::function<int(const int&)> operation;
		std::function<bool(const int&)> test;
		int monkey_if_true;
		int monkey_if_false;

		void turn(std::vector<Monkey>& monkeys, bool div = true);
};

std::vector<Monkey> parse(std::ifstream& input);
void round(std::vector<Monkey>& monkeys, bool div = true);
