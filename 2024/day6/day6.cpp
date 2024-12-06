#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class vec2 {
public:
	int x,y;
	vec2(void) : x(0), y(0) {};
	vec2(std::string str) {
		x = std::stoi(str.substr(0, str.find(',')));
		y = std::stoi(str.substr(str.find(',') + 1, str.size()));
	};
	vec2(int x1, int y1) : x(x1), y(y1) {};
	bool operator==(vec2 const &rhs) { return x == rhs.x && y == rhs.y; };
	vec2 operator+=(vec2 const &rhs) { x += rhs.x; y += rhs.y; return *this; };
	vec2 operator-=(vec2 const &rhs) { x -= rhs.x; y -= rhs.y; return *this; };
	vec2 operator+(vec2 const &rhs) { return vec2(x + rhs.x, y + rhs.y); };
	vec2 operator-(vec2 const &rhs) { return vec2(x - rhs.x, y - rhs.y); };
	std::string str(void) { return std::to_string(x) + "," + std::to_string(y); };
};

int solve(std::vector<std::string> &map, vec2 pos, bool p1 = false) {
	int tiles = 1, steps = 0;
	vec2 dir(0, -1);
	while (steps < map.size() * map[0].size()) {
		char dst = map[pos.y][pos.x];
		if (p1 && dst == '.') {
			map[pos.y][pos.x] = 'X';
			tiles++;
		}

		vec2 next = pos + dir;
		if (next.x < 0 || next.x >= map[0].size() ||
				next.y < 0 || next.y >= map.size())
			return tiles;

		dst = map[next.y][next.x];
		if (dst == '#' || dst == '0') {
			dir = vec2(-dir.y, dir.x);
		} else {
			pos = next;
			steps ++;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	int partOne = 0, partTwo = 0;
	std::string line;
	int y = 0;
	std::vector<std::string> map;
	std::ifstream ifs(argc == 2 ? argv[1] : "input");
	vec2 pos;

	while (std::getline(ifs, line)) {
		int x = line.find('^');
		if (x != std::string::npos)
			pos = vec2(x, map.size());
		map.push_back(line);
	}
	for (auto &l: map)
		std::cout << l << std::endl;

	partOne = solve(map, pos, true);
	map[pos.y][pos.x] = '^';

	for (auto &l: map) {
		int idx = 0;
		while((idx = l.find('X', idx)) != std::string::npos) {
			l[idx] = '0';
			partTwo += !solve(map, pos);
			l[idx] = '.';
		}
	}
	std::cout << "Part One: " << partOne << " ?= 5551" << std::endl;
	std::cout << "Part Two: " << partTwo << " ?= 1939" << std::endl;
	return 0;
}
