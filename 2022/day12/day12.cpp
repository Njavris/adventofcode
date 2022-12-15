/*
	g++ day12.cpp -lncursesw -D_GNU_SOURCE -o out
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <list>
#include <array>
#include <fstream>

#include <ncurses.h>
#include <unistd.h>
#include <csignal>

class Input {
	std::ifstream ifs;
	std::string lastLine;
	char lastByte;
public:
	Input(std::string filename) : lastByte(0), lastLine("")
				{ ifs = std::ifstream(filename); };
	int64_t nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
	int64_t nextByte(void) { ifs.get(lastByte); return !ifs.eof(); };
	char getByte(void) { return lastByte; };
};


void sig_handler(int sig) {
	clear();
	refresh();
	endwin();
}

void atexit_do(void) {
	clear();
	refresh();
	endwin();
}

void program_init(void) {
//	signal(SIGINT, sig_handler);
	signal(SIGSEGV, sig_handler);
	signal(SIGKILL, sig_handler);
	if (atexit(atexit_do))
		exit(-1);

	initscr();
	cbreak();
	clear();
	setlocale(LC_ALL, "");
	init_pair(1, COLOR_GREEN, COLOR_RED);
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
}

class Terrain {
	class Position {
	public:
		int x;
		int y;
		Position(void) : x(0), y(0) {};
		Position(int x1, int y1) { x = x1; y = y1; };
		bool operator==(Position const &rhs) { return x == rhs.x && y == rhs.y; };
		std::string str(void) { return std::to_string(x) + "," + std::to_string(y); };
	};
	std::string dirOrder, dirOrderOppo;
	std::vector<std::string> map;
	Position start;
	Position end;
	Position curr;
	std::vector<char> shortestPath;
	std::vector<char> currPath;
	std::vector<Position> currPathPos;
	std::vector<Position> deadEnds;

	std::vector<std::vector<char>> allPaths;
	char elev;
	int maxLen;
	std::ofstream out;

	std::string pathStr(std::vector<char> &path) {
		std::string ret;
		for (char c: path)
			ret.push_back(c);
		return ret;
	};
	char lookElev(char dir) {
		switch(dir) {
		case '^':
			return (curr.y == 0) ? 0 : map[curr.y - 1][curr.x];
		case 'V':
			return (curr.y == (map.size() - 1)) ? 0 : map[curr.y + 1][curr.x];
		case '>':
			return (curr.x == (map[0].size() - 1)) ? 0 : map[curr.y][curr.x + 1];
		case '<':
			return (curr.x == 0) ? 0 : map[curr.y][curr.x - 1];
		}
		return 0;
	};
	char look(char dir) {
		char ret = lookElev(dir);
		char val = ret == 'E' ? 'z' + 1 : ret;
		if (val <= (elev + 1))
			return ret;
		else
			return 0;
	};
	char getOppo(char dir) {
		return dir == '^' ? 'V' :
			dir == 'V' ? '^' :
			dir == '>' ? '<' :
			dir == '<' ? '>' : 0;
	}
	Position getPos(char dir) {
		Position ret = curr;
		switch(dir) {
		case '^':
			ret.y--;
			break;
		case 'V':
			ret.y++;
			break;
		case '>':
			ret.x++;
			break;
		case '<':
			ret.x--;
			break;
		};
		return ret;
	};
	bool beenThere(Position pos) {
		for (Position p: currPathPos)
			if (p == pos)
				return true;
		return false;
	};
	bool ignoreList(Position pos) {
		for (Position p: deadEnds)
			if (p == pos && map[pos.y][pos.x] != 'E')
				return true;
//		if (map[pos.y][pos.x] < (elev - 1))
//			return true;
		return false;
	}
	bool moveTo(char dir) {
		Position dst = getPos(dir);
		if (beenThere(dst) || ignoreList(dst))
			return false;
		curr = dst;
		elev = map[curr.y][curr.x];
//std::cout << dir << " " << curr.str() << " " << elev << std::endl;
		currPath.push_back(dir);
		currPathPos.push_back(curr);
		return true;
	};
	std::vector<char> lookAround(void) {
		std::vector<char> dirs;
		for (int i = 0; i < dirOrder.size(); i++) {
			char val = look(dirOrder[i]);
			if (!val || val == 'S')
				continue;
			if (currPath.size() && currPath[currPath.size() - 1] ==
					dirOrderOppo[i]) {
				continue;
			}
			if (val == 'E') {
				std::vector<char> end;
				end.push_back(dirOrder[i]);
				return end;
			}
			if (val <= (elev + 1))
				dirs.push_back(dirOrder[i]);
		}
		return dirs;
	};
	char backStep(bool deadEnd) {
		char prev;
//std::cout << "BackStep" << std::endl;
		if (!currPath.size())
			return 0;
		if (deadEnd)
			deadEnds.push_back(curr);
		prev = currPath[currPath.size() - 1];
		currPath.pop_back();
		currPathPos.pop_back();
		curr = getPos(getOppo(prev));
		elev = map[curr.y][curr.x];
		return prev;
	};
	void backtrackPath(void) {
		char prev = 0;
		bool backtracking = false;
		program_init();
		while (true) {
			bool forward = true;
			bool end = false;
			int moved = false;
			std::vector<char> dirs = lookAround();
			drawBacktrackMap();

			if (curr == start && !dirs.size())
				break;

			if (!dirs.size() || currPath.size() >= maxLen ||
					prev == dirOrder[dirOrder.size() - 1] ||
					(dirs.size() && dirs[0] == 'S')) {
				prev = 0;
				forward = false;
			}

			if ((dirs.size() == 1) && lookElev(dirs[0]) == 'E') {
				allPaths.push_back(currPath);
				allPaths[allPaths.size() - 1].push_back(dirs[0]);
				out << "Found: \"" << pathStr(currPath);
				out << "\" len: " << currPath.size() << std::endl;
				out << std::endl;

				prev = 0;
				forward = false;
				backtracking = true;
			}

			if (!forward && currPath.size()) {
				prev = backStep(!backtracking);
				continue;
			}

			for (int i = 0; i < dirOrder.size(); i++) {
				if (prev) {
					if (prev == dirOrder[i])
						prev = 0;
					continue;
				}

				for (char d: dirs) {
					if (d == dirOrder[i]) {
						moved = moveTo(d);
						if (!moved) {
							backtracking = true;
							continue;
						}
						backtracking = true;
						break;
					}
				}
				if (moved)
					break;
			}
			if (!moved)
				prev = backStep(!backtracking);
		}
		clear();
		refresh();
		endwin();
	};
	void drawBacktrackMap() {
		static int call = 0;
		if (call++ % 1000000)
			return;
		for (int y = 0; y < map.size(); y++) {
			for (int x = 0; x < map[y].size(); x++) {
				move(y, x);
				addch(map[y][x]);
			}
		}
		for (int i = 0; i < currPathPos.size(); i++) {
				char ch = (i == (currPathPos.size() - 1)) ?
					'#' : currPath[i + 1];
				move(currPathPos[i].y, currPathPos[i].x);
				addch(ch);
		}
		refresh();
	};

	bool allowed(int x1, int y1, int x2, int y2) {
		char elev1 = map[y1][x1];
		char elev2 = map[y2][x2];

		if (elev1 == 'E')
			elev1 = 'z' + 1;
		if (elev2 == 'E')
			elev2 = 'z' + 1;
		if (elev1 == 'S')
			elev1 = 'a';
		if (elev2 == 'S')
			elev2 = 'a';
		if ((elev1 + 1) >= elev2)
			return true;
		return false;
	};

	int shortestDist = 0;
	int shortestDistStart = 0;
	void aStar(void) {
		std::vector<std::vector<int>> distMap(map.size(), std::vector<int>(map[0].size(), 0));
		distMap[end.y][end.x] = 1;

		while(!distMap[start.y][start.x]) {
			std::vector<std::vector<int>> tmp = distMap;
			for (int y = 0; y < distMap.size(); y++) {
				for (int x = 0; x < distMap[0].size(); x++) {
					int dists[4] = {0, 0, 0, 0}, dist = distMap[y][x];

					/* UP */
					if (y != 0 && allowed(x, y, x, y -1))
						dists[0] = distMap[y - 1][x];
					/* DOWN */
					if (y != (distMap.size() - 1) && allowed(x, y, x, y + 1))
						dists[1] = distMap[y + 1][x];
					/* RIGHT */
					if (x != (distMap[0].size() - 1) && allowed(x, y, x + 1, y))
						dists[2] = distMap[y][x + 1];
					/* LEFT */
					if (x != 0 && allowed(x, y, x - 1, y))
						dists[3] = distMap[y][x - 1];

					for (int i = 0; i < sizeof(dists) / sizeof(int); i++)
						if (dists[i] && (dists[i] < dist || !dist))
							dist = dists[i] + 1;
					tmp[y][x] = dist;
				}
			}
			distMap = tmp;
		}
		shortestDist = distMap[start.y][start.x] - distMap[end.y][end.x];
		shortestDistStart = distMap[start.y][start.x];
		for (int y = 0; y < distMap.size(); y++)
			for (int x = 0; x < distMap[0].size(); x++)
				if (map[y][x] == 'a' && distMap[y][x] &&
						distMap[y][x] < shortestDistStart)
					shortestDistStart = distMap[y][x];
		shortestDistStart -= distMap[end.y][end.x];
	};
	void findShortestPath(void) {
		if (!allPaths.size())
			return;
		shortestPath = allPaths[0];
		for (auto it = allPaths.begin(); it != allPaths.end(); it++) {
			if (shortestPath.size() > it->size())
				shortestPath = *it;
		}
	};
public:
	~Terrain() {
		endwin();
		out.close();
	};
	Terrain(std::string filename, std::string moves) : elev('a'), dirOrder(moves) {
		Input in(filename.size() > 0 ? filename : "input");
		out.open("output");

		while (in.nextLine()) {
			std::string ln = in.getLine();
			int s = ln.find('S');
			int e = ln.find('E');
			if (s != std::string::npos)
				start = Position(s, map.size());
			if (e != std::string::npos)
				end = Position(e, map.size());
			out << ln << std::endl;
			map.push_back(ln);
		}
		for (int i = 0; i < dirOrder.size(); i++)
			dirOrderOppo.push_back(getOppo(dirOrder[i]));

		maxLen = map.size() * map[0].size();
		out << std::endl;
		out << "start: " << start.str() << std::endl;
		out << "end: " << end.str() << std::endl;
		curr = start;
		out << "curr: " << curr.str() << std::endl;
		out << "maxLen: " << maxLen << std::endl;


//		backtrackPath();
		aStar();
		findShortestPath();
		out << "Shortest: \"" << pathStr(shortestPath);
		out << "\" len: " << shortestPath.size() << std::endl;

	};
	int partOne() {
		return shortestDist;
	};
	int partTwo() {
		return shortestDistStart;
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	Terrain ter(param, "^V><");
	std::cout << "Part One: " << ter.partOne() << std::endl;
	std::cout << "Part Two: " << ter.partTwo() << std::endl;
	return 0;
}
