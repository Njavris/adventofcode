#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class v2 {
public:
	int x, y;
	v2() : x(0), y(0) {};
	v2(int x, int y) : x(x), y(y) {};
	v2 &operator+=(v2 const &r) { x += r.x; y += r.y; return *this; } 
	v2 operator+(v2 const &r) { v2 ret = *this; return ret += r; } 
	v2 &operator-=(v2 const &r) { x -= r.x; y -= r.y; return *this; } 
	v2 operator-(v2 const &r) { v2 ret = *this; return ret -= r; } 
	bool operator==(v2 const &r) const { return x == r.x && y == r.y; } 
};

void route(vector<vector<int>> &map, v2 curr) {
	v2 dirs[] = { v2(1, 0), v2(0, 1), v2(-1, 0), v2(0, -1) };
	auto &v = map[curr.y][curr.x];

	for (auto &d : dirs) {
		v2 p = curr + d;
		auto &t = map[p.y][p.x];
		if (t < 0 || (t && t < v))
			continue;
		t = v + 1;
		route(map, p);
	}
}

void cheatP1(vector<vector<int>> &map, vector<int> &cheats) {
	for (int y = 1; y < map.size() - 1; y++) {
		for (int x = 1; x < map[y].size(); x++) {
			auto &c = map[y][x];
			auto &l = map[y][x - 1];
			auto &r = map[y][x + 1];
			auto &d = map[y + 1][x];
			auto &u = map[y - 1][x];

			if (c >= 0)
				continue;

			if (l > 0 && r > 0) {
				int v = abs(l - r) - 2;
				if (v >= 100)
					cheats.push_back(v);
			}
			if (u > 0 && d > 0) {
				int v = abs(u - d) - 2;
				if (v >= 100)
					cheats.push_back(v);
			}
		}
	}
}

void cheatP2(vector<vector<int>> &map, vector<int> &cheats) {
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<vector<int>> map;
	vector<int> cheats;

	v2 start, finish;
	string ln;
	for (int y = 0 ; ifs >> ln; y ++) {
		map.push_back(vector<int>(ln.size(), 0));
		for (int x = 0; x < ln.size(); x++) {
			if (ln[x] == '#')
				map[y][x] = -1;
			if (ln[x] == 'S')
				start = v2(x, y);
			if (ln[x] == 'E')
				finish = v2(x, y);
		}
	}

	map[start.y][start.x] = 1;
	route(map, start);

	cheatP1(map, cheats);
	cout << map[finish.y][finish.x] << endl;
	cout << "Part One: " << cheats.size() << endl;
	cheats.clear();

	cheatP2(map, cheats);
	cout << "Part Two: " << cheats.size() << endl;



	return 0;
}
