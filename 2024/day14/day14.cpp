/*
	g++ day14.cpp -lSDL2 -DSDL2
	or
	g++ day14.cpp
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <climits>
#ifdef SDL2
#include <SDL2/SDL.h>
#endif

using namespace std;

class V2 {
public:
	int x, y;
	V2() : x(0), y(0) {};
	V2(int x, int y) : x(x), y(y) {};
	V2 operator+(V2 const &r) { return V2(x + r.x, y + r.y); };
	V2 operator+=(V2 const &r) { x += r.x; y += r.y; return *this; } 
	V2 &operator%=(V2 const r) {
		int mX = x % r.x;
		int mY = y % r.y;
		x = mX >= 0 ? mX : mX + r.x;
		y = mY >= 0 ? mY : mY + r.y;
		return *this;
	} 
	V2 operator%(V2 const r) {
		V2 ret = *this;
		return ret %= r;
	} 
	bool operator==(V2 const &r) { return x == r.x && y == r.y; } 
	string str() const {return to_string(x) + "," + to_string(y); }
	friend ostream &operator<<(ostream &os, V2 const &v) { os << v.str(); return os; }
};

V2 operator*(V2 const &r, int l) { return V2(r.x * l, r.y * l); }; 

int distance(V2 const &p1, V2 const &p2) {
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	return  (int)abs(sqrt(dx * dx + dy * dy));
}

class Robot {
public:
	static V2 limit;
	static int partTwo(vector<Robot> &robots);
	V2 pos, vel;
	string str() const {return "p=" + pos.str() + " v=" + vel.str(); }
	friend ostream &operator<<(ostream &os, Robot const &r) { os << r.str(); return os; }
	void step(int cnt) {
		pos += vel * cnt;
		pos = pos % limit;
	}
};

long int safetyFactor(vector<Robot> &robots) {
	long int safety = 1;
	int quadrants[4] = { 0 };
	int midX = Robot::limit.x / 2;
	int midY = Robot::limit.y / 2;
	for (auto &r: robots) {
		if (r.pos.x < midX && r.pos.y < midY)
			quadrants[0] ++;
		if (r.pos.x > midX && r.pos.y < midY)
			quadrants[1] ++;
		if (r.pos.x < midX && r.pos.y > midY)
			quadrants[2] ++;
		if (r.pos.x > midX && r.pos.y > midY)
			quadrants[3] ++;
	}

	for (int i = 0; i < sizeof(quadrants) / sizeof(quadrants[0]); i++)
		safety *= quadrants[i];
	return safety;
}

int partOne(vector<Robot> robots) {
	long int partOne = 1;

	for (auto &r: robots)
		r.step(100);

	return safetyFactor(robots);
}

int Robot::partTwo(vector<Robot> &robots) {
#ifdef SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Failed to init SDL: " << SDL_GetError() << endl;
		return -1;
	};

	SDL_Window *win = SDL_CreateWindow("Part Two",
						SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED,
						limit.x,
						limit.y,
						SDL_WINDOW_SHOWN |
						SDL_WINDOW_RESIZABLE);
	if (!win) {
		cout << "Failed to create a window: " << SDL_GetError() << endl;
		return -2;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1,
				SDL_RENDERER_ACCELERATED |
				SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
		cout << "Failed to create a renderer: " << SDL_GetError() << endl;
		SDL_DestroyWindow(win);
		SDL_Quit();
		return -3;
	}

	SDL_Texture *bufTxt = SDL_CreateTexture(ren,
                           		SDL_PIXELFORMAT_RGB24,
                           		SDL_TEXTUREACCESS_STREAMING, 
                           		limit.x,
                           		limit.y);


	int bufSz = limit.x * limit.y * 3;
	uint8_t *buf = new uint8_t[bufSz];

	bool run = true;
	bool pause = false;
	int seconds = 0;
	int fps = 1000;
	int minAvgDist = INT_MAX;
	while (run) {
		int avgDist = 0;
		if (!pause) {
			for (int i = 0; i < robots.size(); i++) {
				Robot &r1 = robots[i];
				for (int  j = i + 1; j < robots.size(); j++) {
					Robot &r2 = robots[j];
					avgDist += distance(r1.pos, r2.pos);
				}
			}
			avgDist /= robots.size();
			if (avgDist <= minAvgDist) {
				minAvgDist = avgDist;
				cout << "time: " << seconds << " avg distance: " << minAvgDist << endl;
				pause = true;
			}
		}

		SDL_Event event;
		bool prev = false, next = false;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				run = false;
			if (event.type == SDL_KEYDOWN) {
				if ((char)event.key.keysym.sym == 'q')
					run = false;
				if ((char)event.key.keysym.sym == ' ') {
					pause = !pause;
					if (pause)
						cout << seconds + 1 << endl;
				}
				if ((char)event.key.keysym.sym == 'p')
					prev = true;
				if ((char)event.key.keysym.sym == 'n')
					next = true;
				if ((char)event.key.keysym.sym == 'w')
					fps ++;
				if ((char)event.key.keysym.sym == 's')
					fps --;
			}
		}
		memset(buf, 0, bufSz);
		for (auto &r: robots) {
			int idx = (r.pos.y * limit.x + r.pos.x) * 3;
			*((uint32_t *)&buf[idx]) = 0xffffff;
			if (!pause) {
				r.step(1);
			} else {
				r.step(prev ? -1 : next ? 1 : 0);
			}
		}
		if (!pause || (pause && (prev || next))) {
			seconds += prev ? -1 : 1;
			if (pause)
				cout << seconds << endl;
		}
		SDL_UpdateTexture(bufTxt, NULL, buf, limit.x * 3);
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, bufTxt, NULL, NULL);
		SDL_RenderPresent(ren);
		SDL_Delay(1000 / fps);
	}

	SDL_DestroyWindow(win);
	SDL_Quit();
	delete[] buf;
#else
	int minAvgDist = INT_MAX;
	long int seconds = 0;
	while (true) {	
		int avgDist = 0;
		for (int i = 0; i < robots.size(); i++) {
			Robot &r1 = robots[i];
			for (int  j = i + 1; j < robots.size(); j++) {
				Robot &r2 = robots[j];
				avgDist += distance(r1.pos, r2.pos);
			}
		}
		avgDist /= robots.size();
		if (avgDist <= minAvgDist) {
			minAvgDist = avgDist;
			cout << "time: " << seconds << " avg distance: " << minAvgDist << endl;
		}
		for (auto &r: robots)
			r.step(1);
		seconds ++;
	}
#endif
	return 0;
}
V2 Robot::limit(101,103);

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<Robot> robots;
	if (argc == 2)
		Robot::limit = V2(11, 7);

	for (string s; getline(ifs,s);) {
		robots.push_back(Robot());
		V2 &p = robots.back().pos;
		V2 &v = robots.back().vel;
		sscanf(s.c_str(), "p=%d,%d v=%d,%d", &p.x, &p.y, &v.x, &v.y);
	}

	cout << "Part One: " << partOne(robots) << endl;

	Robot::partTwo(robots);
	return 0;
}
