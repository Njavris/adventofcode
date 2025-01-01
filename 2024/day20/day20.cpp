#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class v2 {
public:
	int x, y;
	v2() : x(0), y(0) {};
	v2(int x, int y) : x(x), y(y) {};
	v2 &operator+=(v2 const &r) { x += r.x; y += r.y; return *this; } 
	v2 operator+(v2 const &r) { v2 ret = *this; return ret += r; } 
};

v2 dirs[] = { v2(1, 0), v2(0, 1), v2(-1, 0), v2(0, -1) };

void route(vector<vector<int>> &map, v2 curr) {
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

void backtrack(vector<vector<int>> &map, v2 p, vector<v2> &path) {
	auto &v = map[p.y][p.x];
	path.push_back(p);

	for (auto &d : dirs) {
		v2 np = p + d;
		auto &nv = map[np.y][np.x];
		if (nv == v - 1)
			return backtrack(map, np, path);
	}
}

int cheatP1(vector<vector<int>> &map, vector<v2> &path) {
	int ret = 0;
	for (int i = 0; i < path.size(); i++) {
		v2 &p1 = path[i];
		int &v1 = map[p1.y][p1.x];
		for (int j = i + 1; j < path.size(); j++) {
			v2 &p0 = path[j];
			int &v0 = map[p0.y][p0.x];
			int dX = p1.x - p0.x;
			int dY = p1.y - p0.y;
			int wX = 0, wY = 0;
			if (dY == 0 && abs(dX) == 2)
				wX = dX / 2 > 0 ? p1.x + 1 : p1.x - 1; 
			if (dX == 0 && abs(dY) == 2)
				wY = dY / 2 > 0 ? p1.y + 1 : p1.y - 1; 
			if (((wX || wY) && (map[wY][wX] == -1)) && ((v1 - v0 - 1) >= 100))
				ret++;
		}
	}
	return ret;
}

int cheatP2(vector<vector<int>> &map, vector<v2> &path) {
	int ret = 0;
	for (int i = 0; i < path.size(); i++) {
		v2 &p1 = path[i];
		int &v1 = map[p1.y][p1.x];
		for (int j = i + 1; j < path.size(); j++) {
			v2 &p0 = path[j];
			int &v0 = map[p0.y][p0.x];
			int dXY = abs(p0.x - p1.x) + abs(p0.y - p1.y);
			if (dXY > 20)
				continue;
			if (v1 - v0 - dXY >= 100)
				ret ++;
		}
	}
	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<vector<int>> map;
	vector<v2> path;

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
	backtrack(map, finish, path);

	cout << "Part One: " << cheatP1(map, path) << endl;
	cout << "Part Two: " << cheatP2(map, path) << endl;

	return 0;
}
