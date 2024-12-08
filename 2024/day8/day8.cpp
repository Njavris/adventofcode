#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class ant {
public:
	int x,y;
	char freq;
	ant(void) : x(0), y(0) {};
	ant(string str) {
		x = stoi(str.substr(0, str.find(',')));
		y = stoi(str.substr(str.find(',') + 1, str.size()));
	};
	ant(int x1, int y1, char freq) : x(x1), y(y1), freq(freq) {};
	bool operator==(ant const &rhs) { return x == rhs.x && y == rhs.y; };
	ant operator+=(ant const &rhs) { x += rhs.x; y += rhs.y; return *this; };
	ant operator-=(ant const &rhs) { x -= rhs.x; y -= rhs.y; return *this; };
	ant operator+(ant const &rhs) { return ant(x + rhs.x, y + rhs.y, freq); };
	ant operator-(ant const &rhs) { return ant(x - rhs.x, y - rhs.y, freq); };
	ant operator*(int const &val) { return ant(val * x, val * y, freq); };
	bool operator>(ant const &rhs) { return x > rhs.x && y > rhs.x; };
	bool operator>=(ant const &rhs) { return x >= rhs.x && y >= rhs.x; };
	bool operator<(ant const &rhs) { return x < rhs.x && y < rhs.x; };
	bool operator<=(ant const &rhs) { return x <= rhs.x && y <= rhs.x; };
	string str(void) { return to_string(x) + "," + to_string(y) + " - " + freq; };
};

int main(int argc, char **argv) {
	ant limit0(0, 0, -1);
	ant limit1(0, 0, -1);
	vector<ant> antennas, partOne, partTwo;
	ifstream ifs(argc == 2 ? argv[1] : "input");
	for (int y = 0;; y++) {
		string line;
		if (!getline(ifs, line))
			break;
		cout << line << endl;
		for (int x = 0; x < line.length(); x++) {
			char &c = line[x];
			if (c != '.')
				antennas.push_back(ant(x, y, c));
		}
		limit1.x = line.size();
		limit1.y ++;
	}

	for (vector<ant>::iterator it = begin(antennas); it != end(antennas); it++) {
		for (vector<ant>::iterator jt = it + 1; jt != end(antennas); jt++) {
			if (it->freq != jt->freq)
				continue;
			ant node1 = *it;
			ant node2 = *jt;
			ant delta = node1 - node2;
			int cnt = 0;
			while (node1 >= limit0 && node1 < limit1) {
				if (cnt ++ == 1 && find(begin(partOne), end(partOne), node1) == end(partOne))
					partOne.push_back(node1);
				if (find(begin(partTwo), end(partTwo), node1) == end(partTwo))
					partTwo.push_back(node1);
				node1 += delta;
			}
			cnt = 0;
			while (node2 >= limit0 && node2 < limit1) {
				if (cnt ++ == 1 && find(begin(partOne), end(partOne), node2) == end(partOne))
					partOne.push_back(node2);
				if (find(begin(partTwo), end(partTwo), node2) == end(partTwo))
					partTwo.push_back(node2);
				node2 -= delta;
			}
		}
	}

	cout << "Part One: " << partOne.size() << endl;
	cout << "Part Two: " << partTwo.size() << endl;
	return 0;
}
