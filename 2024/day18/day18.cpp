#include <iostream>
#include <fstream>
#include <vector>
#include <array>

using namespace std;

int aStar(vector<vector<int>> map, int dim) {
	int dirs[][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
	map[0][0] = 1;
	int i = 0;
	while (!map[dim - 1][dim - 1] && i++ < (dim * dim) / 2) {
		int delta = 0;
		for (int y = 0; y < dim; y++) {
			for (int x = 0; x < dim; x++) {
				int &t = map[y][x];
				if (t <= 0)
					continue;
				for (int d = 0; d < sizeof(dirs) / sizeof(*dirs); d++) {
					int nX = x + dirs[d][1];
					int nY = y + dirs[d][0];
					if (nX < 0 || nY < 0 || nX >= dim || nY >= dim)
						continue;
					int &n = map[nY][nX];
					if (n < 0)
						continue;
					if (!n || (n > t + 1)) {
						n = t + 1;
						delta ++;
					}
				}
			}
		}
		if (!delta)
			break;
	}
	int &end = map[dim - 1][dim - 1];
	return end ? end - map[0][0] : 0;
}

void solve(vector<array<int, 2>> &bytes, int limit, int dim) {
	vector<vector<int>> map(dim, vector<int>(dim, 0));

	for (int i = 0; i < limit; i++)
		map[bytes[i][1]][bytes[i][0]] = - 1;

	cout << "PartOne: " << aStar(map, dim) << endl;

	for (int i = limit; i < bytes.size(); i++) {
		map[bytes[i][1]][bytes[i][0]] = - 1;
		if (!aStar(map, dim)) {
			cout << "PartTwo: " << bytes[i][0] << "," << bytes[i][1] << endl;
			return;
		}
	}
}

int main(int argc, char ** argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");

	int dim = (argc == 2) ? 7 : 71;
	int limit = (argc == 2) ? 12 : 1024;
	vector<array<int, 2>> in;

	string str;
	for (string str; ifs >> str;) {
		int x,y;
		sscanf(str.c_str(), "%d,%d", &x, &y);
		in.push_back({x, y});
	}

	solve(in, limit, dim);

	return 0;
}
