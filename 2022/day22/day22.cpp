#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cstring>

#include "utils.h"

class Vec {
public:
	int x, y, z;
	Vec() : x(0), y(0), z(0) {};
	Vec(int x1, int x2, int x3) : x(x1), y(x2), z(x3) {};
	std::string toStr(void) {
		return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
	};
	Vec &operator+=(const Vec &s) { x += s.x; y += s.y; z += s.z; return *this; };
	friend Vec operator+(Vec l, const Vec &r) { l += r; return l; };
	Vec &operator*=(const int c) { x *= c; y *= c; z *= c; return *this; };
	friend Vec operator*(Vec l, const int c) { l *= c; return l; };
	Vec &operator*=(const Vec &s) { x *= s.x; y *= s.y; z *= s.z; return *this; };
	friend Vec operator*(Vec l, const Vec &r) { l.x *= r.x; l.y *= r.y; l.z *= r.z; return l; };
	Vec cross(const Vec &s) {
		return Vec(y * s.z - z * s.y, -1 * (x * s.z - z * s.x), x * s.y - y * s.x);
	};
	Vec normal(void) {
		return Vec(x ? (x / std::abs(x)) : 0, y ? (y / std::abs(y)) : 0, z ? (z / std::abs(z)) : 0);
	};
};

class Mat {
public:
	union {
		/*
			0 1 2
			3 4 5
			6 7 8
		*/
		int el[9];
		Vec col[3];
	};
	Mat() { for (int i = 0; i < sizeof(el) / sizeof(el[0]); i++) el[i] = 0; };
	Mat& multiply(const Mat &m) {
		int res[9];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				int val = 0;
				for (int k = 0; k < 3; k++)
					val += el[i + (k * 3)] * m.el[k + (i * 3)];
				res[i + (j * 3)] = val;
			}
		}
		for (int i = 0; i < sizeof(el) / sizeof(el[0]); i++) el[i] = res[i];
		return *this;
	};
	Mat& operator*=(const Mat &m) { return multiply(m); };
	Vec multiply(const Vec &v) {
		return Vec(	el[0] * v.x + el[1] * v.y + el[2] * v.z,
				el[3] * v.x + el[4] * v.y + el[5] * v.z,
				el[6] * v.x + el[7] * v.y + el[8] * v.z);
	};
	static inline int sinSq(int angle) {
		if (angle == 0 || angle == 180 || angle == 360 || angle == -180 || angle == -360)
			return 0;
		if (angle == 90 || angle == -270)
			return 1;
		if (angle == -90 || angle == 270)
			return -1;
		std::cout << "Something Wrong! " << angle << std::endl;
		exit(0);
	};
	static inline int cosSq(int angle) {
		if (angle == 0 || angle == 360 || angle == -360)
			return 1;
		if (angle == 180 || angle == -180)
			return -1;
		if (angle == 90 || angle == -270 || angle == -90 || angle == 270)
			return 0;
		std::cout << "Something Wrong! " << angle << std::endl;
		exit(0);
	};
	static Mat rotMat(int a, int b, int g) {
		Mat rot = Mat();
		rot.el[0] = cosSq(b) * cosSq(g);
		rot.el[1] = sinSq(a) * sinSq(b) * cosSq(g) - cosSq(a) * sinSq(g);
		rot.el[2] = cosSq(a) * sinSq(b) * cosSq(g) + sinSq(a) * sinSq(g);

		rot.el[3] = cosSq(b) * sinSq(g);
		rot.el[4] = sinSq(a) * sinSq(b) * sinSq(g) + cosSq(a) * cosSq(g);
		rot.el[5] = cosSq(a) * sinSq(b) * sinSq(g) - sinSq(a) * cosSq(g);

		rot.el[6] = -sinSq(b);
		rot.el[7] = sinSq(a) * cosSq(b);
		rot.el[8] = cosSq(a) * cosSq(b);
		return rot;
	};
	static Mat rotMat(int angle, Vec axis) {
		Mat rot = Mat();
		int cs = cosSq(angle);
		int sn = sinSq(angle);
		rot.el[0] = axis.x * (1 - cs) + cs;
		rot.el[1] = axis.y * axis.x * (1 - cs) + axis.z * sn;
		rot.el[2] = axis.z * axis.z * (1 - cs) - axis.y * sn;

		rot.el[3] = axis.x * axis.y * (1 - cs) - axis.z * sn;
		rot.el[4] = axis.y * (1 - cs) + cs;
		rot.el[5] = axis.z * axis.z * (1 - cs) + axis.x * sn;

		rot.el[6] = axis.x * axis.z * (1 - cs) + axis.y * sn;
		rot.el[7] = axis.y * axis.z * (1 - cs) - axis.x * sn;
		rot.el[8] = axis.z * (1 - cs) + cs;
		return rot;
	}
	std::string toStr(void) {
		std::string ret;
		for (int i = 0; i < sizeof(el) / sizeof(el[0]); i++) {
			if (i && !(i%3))
				ret += "\n";
			ret += std::to_string(el[i]) + " ";
		}
		return ret;
	};
};

class MoMap {
	int64_t partOneVal;
	int64_t partTwoVal;
	std::string instr;
	int yMax = 0, xMax = 0, sz;
	char *map;
	enum {
		eRight = 0,
		eDown = 1,
		eLeft = 2,
		eUp = 3,
	};
	inline int cToIdx(int x, int y, int z) { return x + y * xMax + z * xMax * yMax; };
	inline int cToIdx(const Vec &v) { return cToIdx(v.x, v.y, v.z); };
	void walk(Vec &pos, int dir, std::string steps) {
		Vec d = Vec();
		int s;
		sscanf(steps.c_str(), "%d", &s);
		if (dir == eRight) d.x = 1;
		else if (dir == eDown) d.y = 1;
		else if (dir == eLeft) d.x = -1;
		else if (dir == eUp) d.y = -1;
		for (int i = 0; i < s; i++) {
			Vec nc = pos + d;
			if (d.x) {
				if (nc.x >= xMax) nc.x = 0;
				else if (nc.x < 0) nc.x = xMax - 1;
				else if (!map[cToIdx(nc)]) nc.x = (d.x > 0) ? 0 : xMax - 1;
				else goto wrapped;
			} else if (d.y) {
				if (nc.y >= yMax) nc.y = 0;
				else if (nc.y < 0) nc.y = yMax - 1;
				else if (!map[cToIdx(nc)]) nc.y = (d.y > 0) ? 0 : yMax - 1;
				else goto wrapped;
			}
			for (int j = 0; (j < (d.x ? xMax : yMax)) && !map[cToIdx(nc)]; j++) nc += d;
wrapped:
			if (map[cToIdx(nc)] == '#') break;
			pos = nc;
		}
	};
	std::string getInstrToken(std::string instr, int *off) {
		int i = *off, start = i;
		do {
			if (instr[i] >= '0' && instr[i] <= '9') continue;
			if (i == *off) i++;
			break;
		} while (instr[i++] != std::string::npos);
		*off = i;
		return instr.substr(start, i - start);
	};
	void doPartOne(void) {
		Timer timer = Timer();
		Vec pos = Vec();
		int dir = eRight, off = 0;
		walk(pos, dir, "1");
		while (off < instr.size()) {
			std::string tkn = getInstrToken(instr, &off);
			if (tkn[0] == 'R') dir = (dir + 1) & 3;
			else if (tkn[0] == 'L') dir = (dir - 1) & 3;
			else walk(pos, dir, tkn);
		}
		partOneVal = (pos.y + 1) * 1000 + 4 * (pos.x + 1) + dir;
		if (partOneVal != 181128) std::cout << "Wrong!" << std::endl;
		timer.stopTiming();
		std::cout << "Time taken by part One: " << timer.getTimeStr() << std::endl;
	};

	void doPartTwo(void) {
		Timer timer = Timer();
		Vec pos = Vec();
		int dir = eRight, off = 0;
		walk(pos, dir, "1");
		std::cout << xMax << "," << yMax << " " << sz << std::endl;
		for (int y = 0; y < yMax; y++) {
			for (int x = 0; x < xMax; x++) {
				Mat r = Mat::rotMat(0, 0, 0);
				Vec v = Vec(x, y, 0);
				Vec rot = r.multiply(v);
//				std::cout << v.toStr() << "->" << rot.toStr() << " ";
				char c = map[cToIdx(x, y, 0)];
				if (c) 	std::cout << c << "(" << x << "," << y << ") ";
				else std::cout << "       ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
		for (int y = 0; y < yMax; y++) {
			for (int x = 0; x < xMax; x++) {
				int i = x / sz;
				int j = y / sz;
				char c = map[cToIdx(x,y, 0)];
				if (c) 	std::cout << c << "(" << i << "," << j << ") ";
				else std::cout << "       ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
		/* wrap around X axis */
		for (int y = 0; y < yMax; y++) {
			for (int x = 0; x < xMax; x++) {
				int angle = 90 * (y / sz);
				Vec v = Vec((x % sz), (y % sz), 0);
				Mat rm = Mat::rotMat(angle, Vec(1, 0, 0));
				Vec r = rm.multiply(v);
				r *= Vec(1, 1, -1);
				if (angle == 90 || angle == 180)
					r += Vec(0, sz - 1, 0);
				if (angle == 180 || angle == 270)
					r += Vec(0, 0, sz - 1);
				char c = map[cToIdx(x, y, 0)];
				if (c) 	std::cout << c << r.toStr() << " ";
				else std::cout << "         ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		/* wrap around Y axis */
		for (int y = 0; y < yMax; y++) {
			for (int x = 0; x < xMax; x++) {
				int angle = 90 * (x / sz);
				Vec v = Vec((x % sz), (y % sz), 0);
				Mat rm = Mat::rotMat(angle, Vec(0, 1, 0));
				Vec r = rm.multiply(v);
//				r *= Vec(1, 1, -1);
				if (angle == 90 || angle == 180)
					r += Vec(sz - 1, 0, 0);
				if (angle == 180 || angle == 270)
					r += Vec(0, 0, sz - 1);
				char c = map[cToIdx(x, y, 0)];
				if (c) 	std::cout << c << r.toStr() << " ";
				else std::cout << "         ";
			}
			std::cout << std::endl;
		}
		while (off < instr.size()) {
			std::string tkn = getInstrToken(instr, &off);
		}
		partTwoVal = (pos.y + 1) * 1000 + 4 * (pos.x + 1) + dir;
		if (partTwoVal != 52311)
			std::cout << "Wrong!" << std::endl;
		timer.stopTiming();
		std::cout << "Time taken by part Two: " << timer.getTimeStr() << std::endl;
	};
public:
	MoMap(std::string fileName) : partOneVal(0), partTwoVal(0) {
		Input in = Input(fileName.size() ? fileName : "input");
		std::vector<std::string> input;
		while (in.nextLine()) {
			std::string ln = in.getLine();
			std::cout << ln << std::endl;
			if (!ln.size()) {
				in.nextLine();
				instr = in.getLine();
				continue;
			}
			input.push_back(ln);
			if (ln.size() > xMax)
				xMax = ln.size();
			yMax++;
		}
		map = new char[xMax * yMax];
		memset(map, 0, xMax * yMax);
		sz = ((xMax > yMax) ? xMax : yMax) / 4;
		for (int y = 0; y < input.size(); y++)
			for (int x = 0; x < input[y].size(); x++)
				if (input[y][x] != ' ')
					map[cToIdx(x, y, 0)] = input[y][x];

		doPartOne();
		doPartTwo();
	};
	~MoMap(void) {
		delete map;
	}
	int64_t partOne(void) { return partOneVal; };
	int64_t partTwo(void) { return partTwoVal; };
};

int main(int argc, char **argv) {
	std::string p;
	if (argc > 1)
		p = std::string(argv[1]);
	MoMap m(p);
	std::cout << "Part One: " << m.partOne() << std::endl;
	    std::cout << "Part Two: " << m.partTwo() << std::endl;
	return 0;
}
