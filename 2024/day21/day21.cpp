#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

typedef struct v2 {
	int8_t x, y;
	v2() : x(0), y(0) {};
	v2(int x, int y) : x(x), y(y) {};
	v2 &operator+=(v2 const &r) { x += r.x; y += r.y; return *this; }
	v2 operator+(v2 const &r) { v2 ret = *this; return ret += r; }
	v2 &operator-=(v2 const &r) { x -= r.x; y -= r.y; return *this; }
	v2 operator-(v2 const &r) { v2 ret = *this; return ret -= r; }
	int16_t s() const { return *(int16_t*)this; }
	friend ostream &operator<<(ostream &o, const v2 &v);
} v2;

ostream &operator<<(ostream &o, const v2 &v) {
	o << "(" << (int)v.x << "," << (int)v.y << ")";
	return o;
}

bool operator==(v2 const &f, v2 const &s) { return f.x == s.x && f.y == s.y; }


map<char, v2> dirs {
		{ 'A', v2(0, 0) },
		{ '>', v2(1, 0) },
		{ 'v', v2(0, 1) },
		{ '<', v2(-1, 0) },
		{ '^', v2(0, -1) }};

const char *numeric[] = { "789", "456", "123", " 0A", NULL };
const char *directional[] = { " ^A", "<v>", NULL };

vector<map<string, vector<string>>> dicts;

v2 findKey(const char **keys, char key) {
	for (int y = 0; keys[y]; y++) {
		for (int x = 0; keys[y][x] != '\0'; x++) {
			if (keys[y][x] == key)
				return v2(x, y);
			}
		}
	return v2(-1, -1);
}

void permutate(const char **k, vector<string> &ret, v2 set, v2 st, string curr = "") {
	if (!set.x && !set.y) {
		for (auto &v : curr) {
			st += dirs[v];
			if (k[st.y][st.x] == ' ')
				return;
		}
		curr += "A";
		if (find(begin(ret), end(ret), curr) == end(ret))
			ret.push_back(curr);
		return;
	}
	v2 t = set;
	string s = curr;
	while (t.x) {
		int dX = set.x / abs(set.x);
		t.x -= dX;
		s += dX > 0 ? ">" : "<";
		permutate(k, ret, t, st, s);
	}
	t = set;
	s = curr;
	while (t.y) {
		int dY = set.y / abs(set.y);
		t.y -= dY;
		s += dY > 0 ? "v" : "^";
		permutate(k, ret, t, st, s);
	}
}

void fillDictionary() {
	string nums = "0123456789A";
	dicts.push_back(map<string, vector<string>>());
	for (auto &k : nums) {
		v2 st = findKey(numeric, k);
		for (auto &k1 : nums) {
			string key = string(1, k) + string(1, k1);
			v2 fi = findKey(numeric, k1);
			v2 d = fi - st;
			dicts.back()[key] = vector<string>();
			permutate(numeric, dicts.back()[key], d, st);
		}
	}
	string dirs = "<>^vA";
	dicts.push_back(map<string, vector<string>>());
	for (auto &k : dirs) {
		v2 st = findKey(directional, k);
		for (auto &k1 : dirs) {
			string key = string(1, k) + string(1, k1);
			v2 fi = findKey(directional, k1);
			v2 d = fi - st;
			dicts.back()[key] = vector<string>();
			permutate(directional, dicts.back()[key], d, st);
		}
	}
}

int translate(long int &best, string &cons, int ci, string prod, int maxDepth, int depth = 0) {
	if (depth > maxDepth) {
		if (!best || best > cons.size()) {
			best = cons.size();
		}
		return best;
	}
	if (ci + 1 == cons.size()) {
		return translate(best, prod, 0, "", maxDepth, depth + 1);
	}

	char c = cons[ci];
	char n = cons[ci + 1];
	if (!prod.size()) {
		c = 'A';
		n = cons[0];
	} else {
		ci ++;
	}
	string k{c, n};
	vector<string> &entry = dicts[!!depth][k];
	for (auto &d : entry) {
		translate(best, cons, ci, prod + d, maxDepth, depth);
	}
		

	return best;
} 

long int solve(vector<string> &codes, int depth) {
	long int ret = 0;
	cout << endl;
	for (auto cd: codes) {
		long int best = 0;
		translate(best, cd, 0, "", depth);
		cout << cd << " " << best << endl;
		cd.pop_back();
		ret += best * strtol(cd.c_str(), NULL, 10);
	}
	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<string> in;

	for (string s; ifs >> s;)
		in.push_back(s);

	fillDictionary();
	cout << "Part One: " << solve(in, 2) << endl;
	return 0;
}
