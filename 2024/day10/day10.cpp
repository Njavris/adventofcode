#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
class pos {
public:
	int x,y;
	pos(int x1, int y1) : x(x1), y(y1) {};
	bool operator==(pos const &rhs) { return x == rhs.x && y == rhs.y; };
	pos operator+(pos const &rhs) { return pos(x + rhs.x, y + rhs.y); };
	std::string str(void) { return std::to_string(x) + "," + std::to_string(y); };
};

void search(vector<vector<int>> &map, pos curr, vector<pos> &peaks, vector<pos> &trails, int depth = 0) {
	pos dirs[] = { pos(1, 0), pos(-1, 0), pos(0, 1), pos(0, -1) };
	int h = map[curr.y][curr.x];

	if (h != depth)
		return;
	if (h == 9) {
		trails.push_back(curr);
		if (find(begin(peaks), end(peaks), curr) == end(peaks))
			peaks.push_back(curr);
		return;
	}
	for (int i = 0; i < sizeof(dirs) /sizeof(dirs[0]); i++) {
		pos tmp = curr + dirs[i];
		if (tmp.y < 0 || tmp.y >= map.size() || tmp.x < 0 || tmp.x >= map[tmp.y].size())
			continue;
		search(map, curr + dirs[i], peaks, trails, depth + 1);
	}
}

int main(int argc, char **argv) {
	int partOne = 0, partTwo = 0;
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<vector<int>> map;
	vector<pos> heads;

	for (string ln; getline(ifs, ln);) {
		static int y = 0;
		map.push_back(vector<int>());
		for (int x = 0; x < ln.size(); x++) {
			int h = ln[x] - '0';
			map.back().push_back(h);
			if (!h)
				heads.push_back(pos(x, y));
		}
		y++;
	}

	for (auto &h: heads) {
		vector<pos> peaks, trails;
		search(map, h, peaks, trails);

		partOne += peaks.size();
		partTwo += trails.size();
	}

	cout << "Part One: " << partOne << endl;
	cout << "Part Two: " << partTwo << endl;
	return 0;
}
