#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>

class Input {
	std::ifstream ifs;
	std::string lastLine;
public:
	Input(std::string filename) : lastLine("") { ifs = std::ifstream(filename); };
	int64_t nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
};

class Pachinko {
	class Point {
	public:
		int x,y;
		Point(void) : x(0), y(0) {};
		Point(std::string str) {
			x = std::stoi(str.substr(0, str.find(',')));
			y = std::stoi(str.substr(str.find(',') + 1, str.size()));
		};
		Point(int x1, int y1) : x(x1), y(y1) {};
		bool operator==(Point const &rhs) { return x == rhs.x && y == rhs.y; };
		Point operator+=(Point const &rhs) { x += rhs.x; y += rhs.y; return *this; };
		Point operator+(Point const &rhs) { return Point(x + rhs.x, y + rhs.y); };
		std::string str(void) { return std::to_string(x) + "," + std::to_string(y); };
	};
	std::vector<std::vector<Point>> paths;
	Point min, max;
	int width, height, off;
	std::vector<Point> dirs;
	std::vector<std::vector<char>> cave;
	int partOneCount;
	int partTwoCount;

	bool validCoords(Point p) { return p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y; };
	char getXY(Point p) { return validCoords(p) ? cave[p.y - min.y][p.x - min.x] : 0; };
	char setXY(Point p, char val) { return validCoords(p) ? cave[p.y - min.y][p.x - min.x] = val : 0; };
	void setLine(Point p1, Point p2) {
		if (p1.x != p2.x && p1.y != p2.y) {
			std::cout << __func__ << ": Unexpected input: ";
			std::cout << p1.str() << " " << p2.str() << std::endl;
			exit(1);
		}

		if (p1.x == p2.x) {
			int dist = p2.y - p1.y;
			int sign = dist / std::abs(dist);
			for (int y = p1.y; y != p2.y + sign; y += sign)
				setXY(Point(p1.x, y), '#');
		} else if (p1.y == p2.y) {
			int dist = p2.x - p1.x;
			int sign = dist / std::abs(dist);
			for (int x = p1.x; x != p2.x + sign; x += sign)
				setXY(Point(x, p1.y), '#');
		}
	};
	void setUpPaths(void) {
		for (auto path: paths) {
			Point prev;
			if (!path.size())
				continue;

			prev = path[0];

			if (path.size() == 1) {
				setLine(prev, prev);
				continue;
			}
			for (int i = 1; i < path.size(); i++) {
				setLine(prev, path[i]);
				prev = path[i];
			}
		}
	};
	void setUpCaves(void) {
		min = Point();
		max = Point();
		min.y = 0;
		for (auto path: paths) {
			for (auto point: path) {
				if (!min.x) min.x = point.x;
				if (point.x < min.x) min.x = point.x;
				if (point.x > max.x) max.x = point.x;
				if (point.y > max.y) max.y = point.y;
			}
		}
		std::cout << "min: " << min.str() << " max: " << max.str() << std::endl;
		std::cout << "After adjustment:" << std::endl;
		max.y ++;
		off = max.y;
		min.x -= off;
		max.x += off;
		std::cout << "min: " << min.str() << " max: " << max.str() << std::endl;
		width = max.x - min.x + 1;
		height = max.y - min.y + 1;
		std::cout << "Grid: " << width << "x" << height << std::endl;

		cave = std::vector<std::vector<char>>(height, std::vector<char>(width, '.'));

		setUpPaths();
		if (!dirs.size()) {
			dirs.push_back(Point(0, 1));
			dirs.push_back(Point(-1, 1));
			dirs.push_back(Point(1, 1));
		}
	};
	void pong(Point p) {
		Point d = Point(1,1);
		char pr = getXY(p);
		while (true) {
			setXY(p, pr);
			if (p.x >= max.x) d.x = -1;
			if (p.x <= min.x) d.x = 1;
			if (p.y <= min.y) d.y = 1;
			if (p.y >= max.y) d.y = -1;
			p += d;
			pr = getXY(p);
			setXY(p, '@');
			usleep(100000);
			drawCaves();
		}
	};

	Point dropSand(Point p) {
		if (p.y >= max.y)
			return p;

		for (auto d: dirs) {
			Point tmp = p + d;
			if (getXY(tmp) == '.') {
				return dropSand(tmp);
			}
		}
		return p;
	};
	void simulation(void) {
		partOneCount = 0;
		partTwoCount = 0;
		bool pOneDone = false;
		Point source(500, 0);
		drawCaves();
		while(true) {
			Point p = dropSand(source);
			setXY(p, 'o');
			drawCaves();
			std::cout << p.str() << std::endl;
			std::cout << partOneCount << std::endl;
			std::cout << partTwoCount << std::endl;
			if (!pOneDone && p.y < max.y)
				partOneCount ++;
			else if (p.y == max.y)
				pOneDone = true;
			partTwoCount ++;
			if (p == source)
				return;
		};
	};
	void drawCaves(void) {
		std::cout << "\033[H";
		for (auto r: cave) {
			for (auto t: r)
				std::cout << t;
			std::cout << std::endl;
		}
	};
public:
	Pachinko(std::string fn= "") {
		Input in = Input(fn.size() ? fn : "input");
		while (in.nextLine()) {
			std::string token, line = in.getLine();
			std::stringstream ss(line);
			std::vector<Point> path;
			while (ss >> token) {
				if (token == "->")
					continue;
				path.push_back(Point(token));
			}
			paths.push_back(path);
			std::string cmp;
			for (auto p: path) {
				cmp += p.str();
				cmp += " -> ";
			}
			cmp.resize(line.size());
			if (cmp != line) {
				std::cout << __func__ << ": Something is wrong with input data" << std::endl;
				exit(1);
			}
		};

		setUpCaves();
//		pong(min);
		simulation();
	};
	int partOne(void) {
		return partOneCount;
	};
	int partTwo(void) {
		return partTwoCount;
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	Pachinko pach(param);
	std::cout << "Part One: " << pach.partOne() << std::endl;
	std::cout << "Part Two: " << pach.partTwo() << std::endl;
	return 0;
}
