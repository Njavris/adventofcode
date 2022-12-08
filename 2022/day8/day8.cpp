#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class Input {
	std::ifstream ifs;
	std::string lastLine;
	char lastByte;
public:
	Input(std::string filename) : lastByte(0), lastLine("")
				{ ifs = std::ifstream(filename); };
	int nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
	int nextByte(void) { ifs.get(lastByte); return !ifs.eof(); };
	char getByte(void) { return lastByte; };
};

class Forest {
	std::vector<std::vector<int>> trees;
//	std::vector<std::vector<unsigned>> treesAvail;
//	std::vector<std::vector<int>> treesView;
	int yMax;
	int xMax;
public:
	Forest(void) {
		Input in = Input("input");
		while (in.nextLine()) {
			std::vector<int> row;
			std::string ln = in.getLine();
			for (std::string::iterator it = ln.begin();
						it != ln.end(); it++)
				row.push_back(*it - '0');
			trees.push_back(row);
		}
		yMax = trees.size() - 1;
		xMax = trees[0].size() - 1;
	};
	unsigned treeAvailable(int x, int y) {
		unsigned ret = 0xf;
		int value = trees[y][x];
		/* up - [0..y][x] */
		for (int i = 0; i < y; i++) {
			if (value <= trees[i][x])
				ret &= ~(1 << 0);
		}
		/* down - [y..max][x] */
		for (int i = y + 1; i <= yMax; i++) {
			if (value <= trees[i][x])
				ret &= ~(1 << 1);
		}
		/* left - [y][0..x] */
		for (int i = 0; i < x; i++) {
			if (value <= trees[y][i])
				ret &= ~(1 << 2);
		}
		/* right - [y][x..max] */
		for (int i = x + 1; i <= xMax; i++) {
			if (value <= trees[y][i])
				ret &= ~(1 << 3);
		}
		return ret;
	};
	int mapAvailable(void) {
		int count = 0;
		for (int y = 0; y <= yMax; y++) {
//			std::vector<unsigned> row;
			for (int x = 0; x <= xMax; x++) {
				unsigned avail = treeAvailable(x, y);
//				row.push_back(avail);
				if (avail)
					count ++;
			}
//			treesAvail.push_back(row);
		}
		return count;
	};
	int findScore(int x, int y) {
		int up, down, left, right;
		int value = trees[y][x];
		int i;
		if (!x || !y || x == xMax || y == yMax)
			return 0;

		/* up - [0..y][x] */
		for (i = y - 1; i > 0; i--)
			if (value <= trees[i][x])
				break;
		up = y - i;

		/* down - [y..max][x] */
		for (i = y + 1; i < yMax; i++)
			if (value <= trees[i][x])
				break;
		down = i - y;

		/* left - [y][0..x] */
		for (i = x - 1; i > 0; i--)
			if (value <= trees[y][i])
				break;
		left = x - i;

		/* right - [y][x..max] */
		for (i = x + 1; i < xMax; i++)
			if (value <= trees[y][i])
				break;
		right = i - x;

		return up * down * left * right;
	};
	int mapView(void) {
		int best = 0;
		for (int y = 0; y <= yMax; y++) {
//			std::vector<int> row;
			for (int x = 0; x <= xMax; x++) {
				int score = findScore(x, y);
//				row.push_back(score);
				if (score > best)
					best = score;
			}
//			treesView.push_back(row);
		}
		return best;
	};
};

int main() {
	Forest forest = Forest();
	std::cout << "Part One: " << forest.mapAvailable() << std::endl;
	std::cout << "Part Two: " << forest.mapView() << std::endl;
	return 0;
}
