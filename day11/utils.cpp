#include "utils.h"
#include <regex>
#include <iostream>
#include <cmath>

std::string space = "[[:space:]]*";
std::regex monkey_re {space + "Monkey ([[:digit:]]+):"};
std::regex items_re {space + "Starting items: (.*)"};
std::regex operation_re {space + "Operation: new = (.*) ([^_[:alnum:]]*) (.*)"};
std::regex test_re {space + "Test: divisible by ([[:digit:]]+)"};
std::regex if_true_re {space + "If true: throw to monkey ([[:digit:]]+)"};
std::regex if_false_re {space + "If false: throw to monkey ([[:digit:]]+)"};

std::function<int(const int&)> parse_operand(std::string operand) {
	std::smatch m;
	if(std::regex_match(operand, m, std::regex("old")))
		return {Old{}};
	Int i;
	i.value = std::stoi(operand);
	return i;
}

std::vector<Monkey> parse(std::ifstream &input) {
	std::vector<Monkey> monkeys;
	std::string line;
	std::smatch match;
	for(;std::getline(input, line);) {
		if(std::regex_match(line, match, monkey_re)) {
			Monkey monkey;
			monkey.id = std::stoi(match[1]);
			std::getline(input, line);
			while(line.size() > 0) {
				std::cout << line << std::endl;
				// Parse items
				if(std::regex_match(line, match, items_re)) {
					const std::string items_str = match[1];
					auto begin = items_str.begin();
					while(std::regex_search(
							begin, items_str.end(),
							match,
							std::regex("\\d+"))) {
						for(auto& item : match) {
							monkey.items.push_back(std::stoi(item));
						}
						begin = match[0].second;
					}
				}
				// Parse Operation
				else if (std::regex_match(line, match, operation_re)) {
					std::smatch op_match;
					std::string op_str = match[2];
					if(std::regex_match(op_str, op_match, std::regex("\\+"))) {
						Plus plus;
						plus.left = parse_operand(match[1]);
						plus.right = parse_operand(match[3]);
						monkey.operation = plus;
					} else if(std::regex_match(op_str, op_match, std::regex("\\*"))) {
						Multiplies multiplies;
						multiplies.left = parse_operand(match[1]);
						multiplies.right = parse_operand(match[3]);
						monkey.operation = multiplies;
					}
				}
				// Parse test
				else if (std::regex_match(line, match, test_re)) {
					DivisibleBy div;
					div.i = std::stoi(match[1]);
					monkey.test = div;
				}
				// Parse case true
				else if (std::regex_match(line, match, if_true_re)) {
					monkey.monkey_if_true = std::stoi(match[1]);
				}
				// Parse case false
				else if (std::regex_match(line, match, if_false_re)) {
					monkey.monkey_if_false = std::stoi(match[1]);
				}
				std::getline(input, line);
			}
			monkeys.push_back(monkey);
		}
	}
	return monkeys;
}

void Monkey::turn(std::vector<Monkey>& monkeys, bool div) {
	std::cout << "Monkey " << id << ": ";
	for(auto& item : items)
		std::cout << item << " ";
	std::cout << std::endl;
	std::size_t items_count = this->items.size();
	for(std::size_t i = 0; i < items_count; i++) {
		std::cout << " Monkey inspects an item with a worry level of " << items.front() << "." << std::endl;
		items.front() = operation(items.front());
		std::cout << "    Worry level is set to " << items.front() << std::endl;
		if(div) {
			items.front() = items.front() / 3;
			std::cout << "    Worry level is divided by 3 to " << items.front() << std::endl;
		}
		if(test(items.front())) {
			std::cout << "    Test is true." << std::endl;
			std::cout << "    Item with worry level " << items.front() << " is thrown to " << monkey_if_true << std::endl;
			monkeys[monkey_if_true].items.push_back(items.front());
		} else {
			std::cout << "    Test is false." << std::endl;
			std::cout << "    Item with worry level " << items.front() << " is thrown to " << monkey_if_false << std::endl;
			monkeys[monkey_if_false].items.push_back(items.front());
		}
		items.pop_front();
		test_count++;
	}
}

void round(std::vector<Monkey>& monkeys, bool div) {
	for(auto& monkey : monkeys)
		monkey.turn(monkeys, div);
}
