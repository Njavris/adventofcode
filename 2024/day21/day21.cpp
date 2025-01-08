#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <chrono>

using namespace std;

typedef struct v2 {
	int x, y;
	v2() : x(0), y(0) {};
	v2(int x, int y) : x(x), y(y) {};
	v2 &operator+=(v2 const &r) { x += r.x; y += r.y; return *this; }
	v2 operator+(v2 const &r) { v2 ret = *this; return ret += r; }
	v2 &operator-=(v2 const &r) { x -= r.x; y -= r.y; return *this; }
	v2 operator-(v2 const &r) { v2 ret = *this; return ret -= r; }
	friend ostream &operator<<(ostream &o, const v2 &v);
} v2;

ostream &operator<<(ostream &o, const v2 &v) {
	o << "(" << (int)v.x << "," << (int)v.y << ")";
	return o;
}

string findPath(const char **k, string key) {
	auto findKey = [&k] (char key) -> v2 {
		for (int y = 0; k[y]; y++)
			for (int x = 0; k[y][x] != '\0'; x++)
				if (k[y][x] == key)
					return v2(x, y);
		return v2(-1, -1);
	};
	v2 ill = findKey(' ');
	v2 start = findKey(key[0]);
	v2 finish = findKey(key[1]);
	v2 set = finish - start;
	bool order = true;

	/* these two stole from reddit */
	if (set.y > 0 && set.x >= 0)
		order = false;
	if (set.y < 0 && set.x > 0)
		order = false;

	if (start.y == ill.y && finish.x == ill.x)
		order = false;
	else if (start.x == ill.x && finish.y == ill.y)
		order = true;
	string chset[] = {
			order ? "<>" : "^v",
			!order ? "<>" : "^v" };
	int axis[] = {
			order ? set.x : set.y,
			!order ? set.x : set.y };
	string ret;
	for (int i = 0; i < 2; i++) {
		int d = axis[i] / abs(axis[i]);
		while(axis[i]) {
			axis[i] -= d;
			ret += d < 0 ? chset[i][0] : chset[i][1];
		}
	}
	return ret + "A";
}

const char *numeric[] = { "789", "456", "123", " 0A", NULL };
const char *directional[] = { " ^A", "<v>", NULL };
map<string, string> pathCache;
map<string, long int> translationCache[25];
map<string, string> keymap;

string pathfind(string &key) {
	if (pathCache.count(key))
		return pathCache[key];

	if (key.size() == 2) {
		if (!keymap.count(key))
			keymap[key] = findPath(directional, key);
		pathCache[key] = keymap[key];
		return pathCache[key];
	}

	string k1 = key.substr(0, key.size() - 1);
	string k2 = key.substr(key.size() - 2);
	string r1, r2;
	if (pathCache.count(k1))
		r1 = pathCache[k1];
	else
 		r1 = pathfind(k1);

	if (pathCache.count(k2))
		r2 = pathCache[k2];
	else
		r2 = pathfind(k2);

	pathCache[key] = r1 + r2;

	return pathCache[key]; 
}

long int translate(string &cons, int maxDepth, int depth = 0) {
	long int ret = 0;
	if (depth == maxDepth)
		return cons.size();

	if (translationCache[depth].count(cons))
		return translationCache[depth][cons];

	vector<string> keys;
	int start = 0;
	for (int i = 0; i < cons.size(); i++) {
		if (cons[i] == 'A') {
			string subs = "A" + cons.substr(start, i - start + 1);
			string tr = pathfind(subs);
			ret += translate(tr, maxDepth, depth + 1);
			start = i + 1;
		}
	}
	translationCache[depth][cons] = ret;
	return ret;
} 

long int solve(vector<string> &codes, int depth) {
	long int ret = 0;
	for (auto &c : translationCache)
		c.clear();
	auto nums = [] (string code) -> string {
		string ret;
		string key = "A";
		while (code.size()) {
			key += code[0];
			ret += findPath(numeric, key);
			code.erase(0, 1);
			key.erase(0, 1);
		}
		return ret;
	};
	for (auto cd: codes) {
		long int best = 0;
		string dirs = nums(cd);
		best = translate(dirs, depth);
		cd.pop_back();
		ret += best * strtol(cd.c_str(), NULL, 10);
	}
	return ret;
}

class Timer {
	bool stopped;
	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;
public:
	Timer(void) : stopped(false) { startTiming(); };
	void startTiming(void) {
		stopped = false;
		start = chrono::high_resolution_clock::now();
	};
	void stopTiming(void) {
		end = chrono::high_resolution_clock::now();
		stopped = true;
	};
	string getTimeStr(void) {
		string ret = "";
		if (!stopped)
			return "";
		auto durationUs = chrono::duration_cast<chrono::microseconds>(end - start);
		auto durationMs = chrono::duration_cast<chrono::milliseconds>(end - start);
		auto durationS = chrono::duration_cast<chrono::seconds>(end - start);
		auto durationM = chrono::duration_cast<chrono::minutes>(end - start);
		auto durationH = chrono::duration_cast<chrono::hours>(end - start);
		chrono::duration<long, micro> intUs = durationUs;
		chrono::duration<long, milli> intMs = durationMs;
		chrono::duration<long, ratio<1>> intS = durationS;
		chrono::duration<long, ratio<60>> intM = durationM;
		chrono::duration<long, ratio<3600>> intH = durationH;

		if (intMs.count()) {
			if (intS.count()) {
				if (intM.count()) {
					if (intH.count()) {
						ret += to_string(intH.count()) + "h ";
					}
					ret += to_string(intM.count() % 60) + "m ";
				}
				ret += to_string(intS.count() % 60) + "s ";
			}
			ret += to_string(intMs.count() % 1000) + "ms ";
		}
		ret += to_string(intUs.count() % 1000) + "us ";
		ret += "(" + to_string(intUs.count()) + "us total)";
		return ret;
	};
};

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<string> in;

	for (string s; ifs >> s;)
		in.push_back(s);

	Timer t;
	t.startTiming();
	long int partOne = solve(in, 2);
	t.stopTiming();
	cout << "Part One: " << partOne << endl;
	cout << t.getTimeStr() << endl;

	t.startTiming();
	long int partTwo = solve(in, 25);
	t.stopTiming();
	cout << "Part Two: " << partTwo << endl;
	cout << t.getTimeStr() << endl;
	return 0;
}
