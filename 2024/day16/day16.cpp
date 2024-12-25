#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

class V2 {
public:
	int x, y;
	V2() : x(0), y(0) {};
	V2(int x, int y) : x(x), y(y) {};
	V2 &operator+=(V2 const &r) { x += r.x; y += r.y; return *this; } 
	V2 operator+(V2 const &r) {
		V2 ret = *this;
		return ret += r;
	} 
	V2 &operator-=(V2 const &r) { x -= r.x; y -= r.y; return *this; } 
	V2 operator-(V2 const &r) {
		V2 ret = *this;
		return ret -= r;
	} 
	bool operator==(V2 const &r) const { return x == r.x && y == r.y; } 
};

V2 dirs[] = { V2(1, 0), V2(0, 1), V2(-1, 0), V2(0, -1) };

int mod(int a, int b) {
	return (a % b) >= 0 ? a % b : a % b + b;
}

int min(vector<int> &tile) {
	int ret = ~0u >> 1;
	for (auto &d: tile)
		if (d && d < ret)
			ret = d;
	return ret == (~0u >> 1) ? -1 : ret;
}

int solveP1(vector<vector<vector<int>>> &map, V2 start, V2 end) {
	auto prev = map;
	auto relaxed = [&map, &prev] () -> bool {
		if (map == prev)
			return true;
		prev = map;
		return false;
	};

	auto turn = [&map] (V2 &pos, int d) {
		int turns[] = { 1, -1 };
		auto &tile = map[pos.y][pos.x];
		int val = tile[d] + 1000;
		for (int t = 0; t < sizeof(turns) / sizeof(*turns); t++) {
			int didx = mod(d + turns[t], tile.size());
			if (!tile[didx] || tile[didx] > val)
				tile[didx] = val;
		}
	};

	map[start.y][start.x][0] = 1;
	turn(start, 0);

	while(!relaxed()) {
		for (int y = 0; y < map.size(); y++) {
			for (int x = 0; x < map[y].size(); x++) {
				auto &tile = map[y][x];
				if (tile[0] < 0)
					continue;
				for (int d = 0; d < tile.size(); d++) {
					if (!tile[d])
						continue;
					V2 nPos = dirs[d] + V2(x, y);
					int &n = map[nPos.y][nPos.x][d];
					int &t = map[y][x][d];
					if (!n || n > t + 1) {
						n = t + 1;
						turn(nPos, d);
					}
				}
			}
		}
	}

	return min(map[end.y][end.x]) - map[start.y][start.x][0];
}

void lookAt(vector<vector<vector<int>>> &map, vector<V2> &tiles, V2 start, V2 pos, int score, int d) {
	int turns[] = { 0, 1, -1 };
	int prices[] = { 1, 1001, 1001 };
	auto &tile = map[pos.y][pos.x];

	tiles.push_back(pos);
	if (pos == start)
		return;

	for (int t = 0; t < sizeof(turns) / sizeof(*turns); t++) {
		int didx = mod(d + turns[t], tile.size());
		V2 nPos = pos - dirs[didx];
		int &nVal = map[nPos.y][nPos.x][didx];
		int expected = score - prices[t];
		if (nVal != expected)
			continue;
		lookAt(map, tiles, start, nPos, expected, didx);
	}
}

int solveP2(vector<vector<vector<int>>> &map, V2 start, V2 end) {
	vector<V2> tiles;
	auto &endT = map[end.y][end.x];
	int m = min(endT);

	for (int i = 0; i < sizeof(dirs) / sizeof(*dirs); i++) {
		if (endT[i] == m)
			lookAt(map, tiles, start, end, m, i);
	}

	for (int i = 0; i < tiles.size(); i++) {
		for (int j = i + 1; j < tiles.size(); j++) {
			if (tiles[j] == tiles[i])
				tiles.erase(begin(tiles) + j--);
		}
	}
	return tiles.size();
}


int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<vector<vector<int>>> map;

	V2 start, end;

	string str;
	for (int y = 0, pos = 0; ifs >> str; y++) {
		map.push_back(vector<vector<int>>());
		for (auto &x: str) {
			if (x == '#')
				map.back().push_back(vector<int>(4, -1));
			else
				map.back().push_back(vector<int>(4, 0));
		}
		if ((pos = str.find('S')) != string::npos)
			start = V2(pos, y);
		if ((pos = str.find('E')) != string::npos)
			end = V2(pos, y);
	}

	cout << "Part One: " << solveP1(map, start, end) << endl;
	cout << "Part Two: " << solveP2(map, start, end) << endl;

	return 0;
}
