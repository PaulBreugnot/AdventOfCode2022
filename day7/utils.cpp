#include "utils.h"
#include <regex>
#include <fstream>
#include <iostream>

const std::regex cd_regex {"\\$ cd (.*)"};
const std::regex cmd_regex {"\\$ .*"};
const std::regex dir_regex {"dir (.*)"};
const std::regex file_regex {"([[:digit:]]+) (.*)"};

void explore(Node& node, std::string line, std::ifstream& input) {
	std::smatch matcher;
	if(line.size()>0) {
		if(std::regex_match(line, matcher, cd_regex)) {
			std::string dest = matcher[1];
			std::getline(input, line);
			if(dest == "..")
				explore(*node.parent, line, input);
			else
				explore(node.children[dest], line, input);
		} else {
			std::getline(input, line);
			while(line.size() > 0 && !std::regex_match(line, matcher, cmd_regex)) {
				Node child;
				child.parent = &node;
				if(std::regex_match(line, matcher, dir_regex)) {
					child.type = T_DIR;
					child.size = 0;
					child.name = matcher[1];
					node.children[child.name] = child;
				} else if (std::regex_match(line, matcher, file_regex)) {
					child.type = T_FILE;
					child.size = std::stoi(matcher[1]);
					child.name = matcher[2];
					node.children[child.name] = child;
				}
				std::getline(input, line);
			}
			explore(node, line, input);
		}
	}
}

std::size_t size(Node& node) {
	for(auto& item : node.children) {
		node.size += size(item.second);
	}
	return node.size;
}

Node build_filesystem(std::ifstream& input) {
	Node root;
	root.name = "SYSTEM";
	Node base;
	base.type = T_DIR;
	base.name = "/";
	base.size = 0;
	root.children["/"] = base;
	std::string line;
	std::getline(input, line);
	explore(root, line, input);
	size(root);
	return root.children["/"];
}

void display(const Node& node, int level) {
	for(int i = 0; i < level; i++)
		std::cout << "  ";
	std::cout << "- " << node.name << " (";
	switch(node.type) {
		case T_FILE:
			std::cout << "file, size=" << node.size << ")" << std::endl;
		break;
		case T_DIR:
			std::cout << "dir, size=" << node.size << ")" << std::endl;
			for(auto& child : node.children)
				display(child.second, level+1);
	}
}

