#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class v2 {
public:
	int x, y;
	v2() : x(0), y(0) {};
	v2(int x, int y) : x(x), y(y) {};
	v2 &operator++(int) { x ++; y++; return *this; }
	v2 &operator+=(v2 const &r) { x += r.x; y += r.y; return *this; }
	v2 operator+(v2 const &r) { v2 ret = *this; return ret += r; }
	v2 &operator-=(v2 const &r) { x -= r.x; y -= r.y; return *this; }
	v2 operator-(v2 const &r) { v2 ret = *this; return ret -= r; }
	friend ostream &operator<<(ostream &o, const v2 &v);
};

ostream &operator<<(ostream &o, const v2 &v) {
	o << "(" << (int)v.x << "," << (int)v.y << ")";
	return o;
}

int mod(int a, int b) {
	return (a % b) >= 0 ? a % b : a % b + b;
}

int partOne(vector<string> &board, string directions) {
	v2 dirs[] = { v2(1, 0), v2(0, 1), v2(-1, 0), v2(0, -1) };
	int dir = 0;
	v2 pos;
	auto rot = [&dir] (char d) {
		string dirch = ">v<^";
		cout << d << ": " << dirch[dir] << "(" << dir << ") -> ";
		dir += d == 'R' ? 1 : -1;
		dir = mod(dir, 4);
		cout << dirch[dir] << "(" << dir << ")" << endl;
	};
	auto move = [&dir, &dirs, &pos, &board] (int cnt) {
		for (int i = 0; i < cnt; i++) {
			v2 nPos = pos + dirs[dir];
			auto &d = board[nPos.y][nPos.x];
			if (d == '#')
				break;
		}
	};
	pos.x = board.front().find(".");

	cout << endl;
	cout << directions << endl;
	for (int i = 0, start = 0; i < directions.size();) {
		auto &d = directions[i];
		if (d >= '0' && d <= '9') {
			int end = min(directions.find('R', start), directions.find('L', start));
			int l = end - start;
			int cnt = stoi(directions.substr(start, l));
			move(cnt);
			start = i + 1;
			i += l;
		} else if (d == 'L' || d == 'R') {
			start = ++i;
			rot(d);
		}
	}
	cout << endl;

	pos++;
	return pos.y * 1000 + pos.x * 4 + dir;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	string directions;
	vector<string> board;
	for (string s; getline(ifs, s);) {
		if (!s.size())
			break;
		board.push_back(s);
	}
	ifs >> directions;

	cout << "Part One: " << partOne(board, directions) << endl;

	return 0;
}
