#include "utils.h"
#include <iostream>
#include <cassert>

const std::array<Rock, 5> rocks {{
	{
		{
		{1, 1, 1, 1}
		}, 4, 1
	},{
		{
		{0, 1, 0},
		{1, 1, 1},
		{0, 1, 0}
		}, 3, 3
	},{
		{
		{1, 1, 1},
		{0, 0, 1},
		{0, 0, 1}
		}, 3, 3
	},{
		{
		{1},
		{1},
		{1},
		{1}
		}, 1, 4
	},{
		{
		{1, 1},
		{1, 1}
		}, 2, 2
	}
}};

void World::parse(std::ifstream& input) {
	std::string line;
	std::getline(input, line);
	jet_sequence.resize(line.size());
	std::size_t i = 0;
	for(char jet : line) {
		if(jet == '<')
			jet_sequence[i] = LEFT;
		else
			jet_sequence[i] = RIGHT;
		++i;
	}
	std::cout << "Jet sequence length: " << jet_sequence.size() << std::endl;
}

World::World(std::ifstream& input) : cave(4), time(0), tower_top(0), rock_count(0), falling_rock(&rocks[0], {2, 3}) {
	parse(input);
}

Jet World::currentJet() const {
	return jet_sequence[time%jet_sequence.size()];
}

void World::spawnRock() {
	const Rock& rock = rocks[rock_count%5];
	std::size_t new_height = tower_top+3+rock.height;
	if(cave.size() < new_height)
		cave.resize(new_height);
	falling_rock = {
		&rock, {2, tower_top+3}
	};
}

void World::pushRock() {
	Jet current_jet = currentJet();
	switch(current_jet) {
		case LEFT:
			if(falling_rock.location().x > 0) {
				bool can_move = true;
				if(falling_rock.location().y <= tower_top) {
					int y = 0;
					while(can_move && y < falling_rock.rock->height) {
						int x = 0;
						while(can_move && x < falling_rock.rock->width) {
							if (cave[falling_rock.location().y+y][falling_rock.location().x+x-1]
									+ falling_rock.rock->shape[y][x] > 1) {
								can_move = false;
							}
							x++;
						}
						y++;
					}
				}
				if(can_move) {
					--falling_rock.location().x;
#ifdef LOG
					std::cout << "Jet of gas pushes rock LEFT" << std::endl;
					std::cout << *this << std::endl;
#endif
				}
			}
			break;
		case RIGHT:
			if(falling_rock.location().x + falling_rock.rock->width < CAVE_WIDTH) {
				bool can_move = true;
				if(falling_rock.location().y <= tower_top) {
					int y = 0;
					while(can_move && y < falling_rock.rock->height) {
						int x = 0;
						while(can_move && x < falling_rock.rock->width) {
							if (cave[falling_rock.location().y+y][falling_rock.location().x+x+1]
									+ falling_rock.rock->shape[y][x] > 1) {
								can_move = false;
							}
							x++;
						}
						y++;
					}
				}
				if(can_move) {
					++falling_rock.location().x;
#ifdef LOG
					std::cout << "Jet of gas pushes rock RIGHT" << std::endl;
					std::cout << *this << std::endl;
#endif
				}
			}
	}
}

bool World::fallRock() {
	if(falling_rock.location().y > 0) {
		bool can_fall = true;
		if(falling_rock.location().y <= tower_top) {
			int y = 0;
			while(can_fall && y < falling_rock.rock->height) {
				int x = 0;
				while(can_fall && x < falling_rock.rock->width) {
					if (cave[falling_rock.location().y+y-1][falling_rock.location().x+x]
							+ falling_rock.rock->shape[y][x] > 1) {
						can_fall = false;
					}
					x++;
				}
				y++;
			}
		}
		if (can_fall) {
			--falling_rock.location().y;
#ifdef LOG
			std::cout << "Rock falls down" << std::endl;
			std::cout << *this << std::endl;
#endif
			return true;
		}
	}
	// Falling rock sleeps
	for(int y = 0; y < falling_rock.rock->height; y++) {
		for(int x = 0; x < falling_rock.rock->width; x++) {
			cave[falling_rock.location().y+y][falling_rock.location().x+x]
				+= falling_rock.rock->shape[y][x];
		}
	}
	++rock_count;
	int rock_top = falling_rock.location().y + falling_rock.rock->height;
	if(rock_top > tower_top)
		tower_top=rock_top;
	return false;
}

void World::step() {
	pushRock();
	if(!fallRock())
		spawnRock();
	++time;
}

void World::drop(std::size_t rock_count) {
	std::cout << "Dropping " << rock_count << " rocks." << std::endl;
	while(this->rock_count < rock_count)
		step();
}

int World::rockCount() const {
	return rock_count;
}

int World::towerTop() const {
	return tower_top;
}

std::ostream& operator<<(std::ostream& o, const World& world) {
	std::vector<std::array<int, World::CAVE_WIDTH>> cave_copy = world.cave;
	for(int y = 0; y < world.falling_rock.rock->height; y++) {
		for(int x = 0; x < world.falling_rock.rock->width; x++) {
			cave_copy[world.falling_rock.location().y+y][world.falling_rock.location().x+x]
				= world.falling_rock.rock->shape[y][x]*2;
		}
	}
	for(auto line = cave_copy.rbegin(); line!=cave_copy.rend(); line++) {
		if(world.currentJet() == RIGHT)
			o << '>';
		else
			o << '|';
		for(auto& cell : *line) {
			if (cell==0)
				o << '.';
			else if (cell==1)
				o << '#';
			else
				o << '@';
		}
		if(world.currentJet() == LEFT)
			o << '<';
		else
			o << '|';
		o << std::endl;
	}
	std::cout << "+-------+" << std::endl;
	return o;
}
