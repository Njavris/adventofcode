#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#ifdef SDL2
#include <SDL2/SDL.h>

class sdl2 {
public:
	int x,y;
	int sz, fps;
	sdl2(int x, int y) : x(x), y(y), fps(120), sz(x * y * sizeof(uint32_t)) {};
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *bufTxt;
	int init() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			cout << "Failed to init SDL: " << SDL_GetError() << endl;
			return -1;
		};
		win = SDL_CreateWindow("Part Two",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					x,
					y,
					SDL_WINDOW_SHOWN |
					SDL_WINDOW_RESIZABLE);
		if (!win) {
			cout << "Failed to create a window: " << SDL_GetError() << endl;
			return -2;
		}

		ren = SDL_CreateRenderer(win, -1,
					SDL_RENDERER_ACCELERATED |
					SDL_RENDERER_PRESENTVSYNC);
		if (!ren) {
			cout << "Failed to create a renderer: " << SDL_GetError() << endl;
			SDL_DestroyWindow(win);
			SDL_Quit();
			return -3;
		}

		bufTxt = SDL_CreateTexture(ren,
                	      		SDL_PIXELFORMAT_ARGB8888,
                           		SDL_TEXTUREACCESS_STREAMING, 
                           		x,
                           		y);
		return 0;
	};
	int deinit() {
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 0;
	};
	int drawBuf(uint32_t *buf) {
		SDL_UpdateTexture(bufTxt, NULL, buf, x * sizeof(uint32_t));
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, bufTxt, NULL, NULL);
		SDL_RenderPresent(ren);
		SDL_Delay(1000 / fps);
		return 0;
	};
	bool getKeyPress(char &key) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return false;
			if (event.type == SDL_KEYDOWN)
				key = (char)event.key.keysym.sym;
		}
		return true;
	};
};
#endif

void moveP1(vector<string> &map, char m, int &rX, int &rY) {
	int dirsVal[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
	string dirsStr = "><v^";
	int d = 0;
	for (; d < sizeof(dirsVal) / sizeof(dirsVal[0]) && dirsStr[d] != m; d ++);

	int nX = rX + dirsVal[d][0];
	int nY = rY + dirsVal[d][1];
	char &n = map[nY][nX];
	char &r = map[rY][rX];
//	cout << m << ": " << rX << "," << rY << "(" << r << ") -> " << nX << "," << nY << "(" << n << ")" << endl;

	if (n == '.') {
		n = '@';
		r = '.';
		rX = nX;
		rY = nY;
	} else if (n == 'O') {
		int nbX = nX;
		int nbY = nY;

		while (true) {
			nbX += dirsVal[d][0];
			nbY += dirsVal[d][1];
			char &nb = map[nbY][nbX];
			if (nb == '.') {
				nb = 'O';
				n = '@';
				r = '.';
				rX = nX;
				rY = nY;
				break;
			} else if (nb == 'O') {
				continue;
			} else {
				break;
			}
		}
	}
}

int printResultsP1(vector<string> &map) {
	int ret = 0;
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].size(); x++) {
			if (map[y][x] == 'O')
				ret += y * 100 + x;
		}
	}
	return ret;
}

void moveP2(vector<string> &map, char m, int &rX, int &rY) {
	int dirsVal[][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
	string dirsStr = "><v^";
	int d = 0;
	for (; d < sizeof(dirsVal) / sizeof(dirsVal[0]) && dirsStr[d] != m; d ++);

	int nX = rX + dirsVal[d][0];
	int nY = rY + dirsVal[d][1];
	char &n = map[nY][nX];
	char &r = map[rY][rX];
	cout << m << ": " << rX << "," << rY << "(" << r << ") -> " << nX << "," << nY << "(" << n << ")" << endl;

	if (n == '.') {
		n = '@';
		r = '.';
		rX = nX;
		rY = nY;
	} else if (d > 1 && (n == '[' || n == ']')) {
		int nbX = nX;
		int nbY = nY;

		while (true) {
			nbX += dirsVal[d][0];
			nbY += dirsVal[d][1];
			char &nb = map[nbY][nbX];
			if (nb == '.') {
				nb = 'O';
				n = '@';
				r = '.';
				rX = nX;
				rY = nY;
				break;
			} else if (nb == 'O') {
				continue;
			} else {
				break;
			}
		}
	}
}

int printResultsP2(vector<string> &map) {
	int ret = 0;
	for (int y = 0; y < map.size(); y++) {
		for (int x = 0; x < map[y].size(); x++) {
			if (map[y][x] == 'O')
				ret += y * 100 + x;
		}
	}
	return ret;
}

void solve(vector<string> map, string &moves, int rX, int rY, int height, int width, bool p2 = false) {
#ifdef SDL2
	sdl2 sdl(width, height);
	sdl.init();
	char key;
	uint32_t *buf = new uint32_t[sdl.sz];
	sdl.fps = 1;
#endif
	int instr = 0;
#ifdef SDL2
	while (sdl.getKeyPress(key)) {
		if (key == 'q')
			break;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				char &c = map[y][x];
				int idx = y * sdl.x + x;
				buf[idx] = c == '#' ? 0xa000a0 :
						c == 'O' ? 0x404040 :
						c == '[' ? 0x404000 :
						c == ']' ? 0x400040 :
						c == '@' ? 0xff4040 : 0x0;
			}
		}
	//	if (!(instr % 50))
		sdl.drawBuf(buf);
#else
	while (true) {
#endif
		char mv = moves[instr++];
		if (!p2)
			moveP1(map, mv, rX, rY);
		else
			moveP2(map, mv, rX, rY);
		if (instr == moves.size()) {
			if (!p2)
				cout << "Part One: " << printResultsP1(map) << endl;
			else
				cout << "Part Two: " << printResultsP2(map) << endl;
			break;
		}
	}
#ifdef SDL2
	sdl.deinit();
	delete[] buf;
#endif
}


int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	string moves;
	int width, height;
	int rX, rY;

	vector<string> map;
	for (string s; getline(ifs,s);) {
		if (!s.size())
			break;
		map.push_back(s);

		int x = s.find('@'); 
		if (x != string::npos) {
			rX = x;
			rY = map.end() - map.begin() - 1;
		}
	}
	for (string s; getline(ifs,s); moves += s);
//	solve(map, moves, rX, rY, map.size(), map[0].size());
	vector<string> map1;
	for (auto &row: map) {
		string tmp;
		for (int x = 0; x < row.size(); x ++) {
			char &c = row[x];
			if (c == '#')
				tmp += "##";
			else if (c == '.')
				tmp += "..";
			else if (c == 'O')
				tmp += "[]";
			else if (c == '@') {
				tmp += "@.";
			}
		}
		map1.push_back(tmp);
		int x = tmp.find('@'); 
		if (x != string::npos) {
			rX = x;
			rY = map1.end() - map1.begin() - 1;
		}
	}
	solve(map1, moves, rX, rY, map1.size(), map1[0].size(), true);


	return 0;
}
