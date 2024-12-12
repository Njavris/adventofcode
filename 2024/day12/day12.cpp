#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Tile {
public:
	int x, y, v, g;
	unsigned per, edg;
	Tile(int x, int y, int v) : x(x), y(y), v(v), g(-1), edg(0xf), per(0xf) {};
};


void fillGroups(vector<vector<Tile>> &map, Tile &t, int g) {
	int dirs[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
	t.g = g;
	for (int i = 0; i < sizeof(dirs) / sizeof(dirs[0]); i++) {
		int x1 = t.x + dirs[i][0];
		int y1 = t.y + dirs[i][1];
		if (y1 < 0 || x1 < 0 || y1 >= map.size() || x1 >= map[0].size())
			continue;
		Tile &t1 = map[y1][x1];
		if (t1.v == t.v) {
			if (t1.g == -1)
				fillGroups(map, t1, g);
			t.edg &= ~((1 << i) & 0xf);
			t.per = t.edg;
		} 
	}
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<vector<Tile>> map;

	string ln;
	for (int y = 0; getline(ifs, ln); y++) {
		map.push_back(vector<Tile>());
		for (int x = 0; x < ln.size(); x++)
			map.back().push_back(Tile(x, y, (int)ln[x]));
	}

	int gId = 0;
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[0].size(); x++) {
			Tile &t = map[y][x];
			if (t.g == -1)
				fillGroups(map, t, gId++);
		}
	}

	vector<int> groups, perimeter, edges;
	groups.resize(gId, 0);
	perimeter.resize(gId, 0);
	edges.resize(gId, 0);

	/* Horisontal edges */
	for (int y = 0; y < map.size(); y++) {
		Tile *prev = NULL;
		for (int x = 0; x < map[0].size(); x++) {
			Tile *t = &map[y][x];
			if (prev && prev->g == t->g)
				prev->edg &= ~(t->edg & prev->edg);
			prev = t;
		}
	}
	/* Vertical edges */
	for (int x = 0; x < map[0].size(); x++) {
		Tile *prev = NULL;
		for (int y = 0; y < map[0].size(); y++) {
			Tile *t = &map[y][x];
			if (prev && prev->g == t->g)
				prev->edg &= ~(t->edg & prev->edg);
			prev = t;
		}
	}

	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[0].size(); x++) {
			Tile &t = map[y][x];
			groups[t.g] ++;
			for (int b = 0; b < 4;
				perimeter[t.g] += (t.per >> b) & 1,
				edges[t.g] += (t.edg >> b++) & 1);
		}
	}

	int partOne = 0, partTwo =0;
	for (int i = 0; i < gId; i++) {
		partOne += perimeter[i] * groups[i];
		partTwo += edges[i] * groups[i];
	}
	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;

	return 0;
}
