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

bool operator==(v2 const &f, v2 const &s) {
	return f.x == s.x && f.y == s.y;
}


map<char, v2> dirs {
		{ 'A', v2(0, 0) },
		{ '>', v2(1, 0) },
		{ 'v', v2(0, 1) },
		{ '<', v2(-1, 0) },
		{ '^', v2(0, -1) }};

const char *numeric[] = { "789", "456", "123", " 0A", NULL };
const char *directional[] = { " ^A", "<v>", NULL };

void permutateV2(const char **k, vector<string> &ret, v2 set, v2 st, string curr = "") {
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
		permutateV2(k, ret, t, st, s);
	}
	t = set;
	s = curr;
	while (t.y) {
		int dY = set.y / abs(set.y);
		t.y -= dY;
		s += dY > 0 ? "v" : "^";
		permutateV2(k, ret, t, st, s);
	}
}

vector<map<string, vector<string>>> dict;

void fillDictionary() {
	dict.push_back(map<string, vector<string>>());
	dict.push_back(map<string, vector<string>>());
	auto findKey = [] (const char **keys, char key) -> v2 {
		for (int y = 0; keys[y]; y++)
			for (int x = 0; keys[y][x] != '\0'; x++)
				if (keys[y][x] == key)
					return v2(x, y);
		return v2(-1, -1);
	};

	string nums = "0123456789A";
	for (auto &k : nums) {
		v2 st = findKey(numeric, k);
		for (auto &k1 : nums) {
			string key = string(1, k) + string(1, k1);
			v2 fi = findKey(numeric, k1);
			v2 d = fi - st;
			dict[0][key] = vector<string>();
			permutateV2(numeric, dict[0][key], d, st);
		}
	}
	string dirs = "<>^vA";
	for (auto &k : dirs) {
		v2 st = findKey(directional, k);
		for (auto &k1 : dirs) {
			string key = string(1, k) + string(1, k1);
			v2 fi = findKey(directional, k1);
			v2 d = fi - st;
			dict[1][key] = vector<string>();
			permutateV2(directional, dict[1][key], d, st);
		}
	}
}

vector<string> &cachedLookup(string &key, int depth) {
	if (dict[!!depth].count(key))
		return dict[!!depth][key];

	map<string, vector<string>> &cache = dict[!!depth];
	if (cache.count(key))
		return cache[key];
	cache[key] = vector<string>();
	for (int i = key.size() - 1; i >= 2; i--) {
		string k = key.substr(0, i);
		if (!cache.count(k)) 
			continue;
		vector<string> &r1 = cache[k];
		string k1 = key.substr(i - 1);
		vector<string> &r2 = cachedLookup(k1, depth);
		for (auto &i1 : r1) {
			for (auto &i2 : r2) {
				cache[key].push_back(i1 + i2);
			}
		}
		break;
	}
	return cache[key]; 
}

int translate(long int &best, string &cons, int maxDepth, int depth = 0) {
	if (depth > maxDepth) {
		if (!best || best > cons.size()) {
			best = cons.size();
		}
		return best;
	}
	string key = "A" + cons;
	vector<string> &tr = cachedLookup(key, depth);

	for (auto &s : tr)
		translate(best, s, maxDepth, depth + 1);
	return best;
} 

long int solve(vector<string> &codes, int depth) {
	long int ret = 0;
	cout << endl;
	for (auto cd: codes) {
		long int best = 0;
		cd = cd;
		translate(best, cd, depth);
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
	cout << "Part Two: " << solve(in, 25) << endl;
	return 0;
}
