#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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
} v2;
bool operator==(v2 const &f, v2 const &s) { return f.x == s.x && f.y == s.y; }

enum eDirs : uint8_t {
	eLeft = 0,
	eRight,
	eUp,
	eDown,
	eA,
	eMax
};

typedef basic_string<eDirs> dirStr;

char dirCh[] = {
	[eLeft] = '<',
	[eRight] = '>',
	[eUp] = '^',
	[eDown] = 'v',
	[eA] = 'A',
};

v2 dirV2[] = {
	[eLeft] = v2(-1, 0),
	[eRight] = v2(1, 0),
	[eUp] = v2(0, -1),
	[eDown] = v2(0, 1),
	[eA] = v2(0, 0),
};

const char *numeric[] = { "789", "456", "123", " 0A", NULL };
const char *directional[] = { " ^A", "<v>", NULL };
v2 *strtNum = NULL;
v2 *strtDir = NULL;

v2 findKey(const char **keys, char key) {
	for (int y = 0; keys[y]; y++) {
		for (int x = 0; keys[y][x] != '\0'; x++) {
			if (keys[y][x] == key)
				return v2(x, y);
		}
	}
	return v2(-1, -1);
}

bool validator(const char **keys, v2 pos, dirStr seq) {
	v2 invalid = findKey(keys, ' ');

	for (auto &v : seq) {
		pos += dirV2[v];
		if (pos == invalid)
			return false;
	}
	return true;
}

void permutator(const char **keys, vector<dirStr> &ret, v2 set, v2 pos, dirStr curr = dirStr()) {
	if (!set.x && !set.y) {
		if (validator(keys, pos, curr) && find(begin(ret), end(ret), curr) == end(ret))
			ret.push_back(curr);
		return;
	}
	v2 tmp = set;
	dirStr s = curr;
	while (tmp.x) {
		int dX = set.x / abs(set.x);
		int cnt = !tmp.y ? abs(tmp.x) : 1;
		for (int i = 0; i < cnt; i++) {
			s += dX > 0 ? eRight : eLeft;
			tmp.x -= dX;
		}
		permutator(keys, ret, tmp, pos, s);
	}
	tmp = set;
	s = curr;
	while (tmp.y) {
		int dY = set.y / abs(set.y);
		int cnt = !tmp.x ? abs(tmp.y) : 1;
		for (int i = 0; i < cnt; i++) {
			s += dY > 0 ? eDown : eUp;
			tmp.y -= dY;
		}
		permutator(keys, ret, tmp, pos, s);
	}
}

void flatenator(vector<dirStr> &ret, vector<dirStr> &app) {
	if (!ret.size()) {
		for (auto &a : app) {
			ret.push_back(a + eA);
		}
		return;
	}
	int sz = ret.size();
	for (int i = 0; i < sz; i++) {
		dirStr tmp = ret[0];
		ret.erase(begin(ret));
		for (int j = 0; j < app.size(); j++) {
			ret.push_back(tmp + app[j] + eA);
		}
	}
}

void translator(vector<dirStr> &ret, const char **keys, string input, v2 pos) {
	for (auto &c : input) {
		vector<dirStr> tmp;
		v2 next = findKey(keys, c);
		v2 d = next - pos;
		permutator(keys, tmp, d, pos);
		pos = next;
		flatenator(ret, tmp);
	}
}

v2 *keyLookup = NULL;
void translator(vector<dirStr> &ret, const char **keys, dirStr input, v2 pos) {
	for (auto &c : input) {
		vector<dirStr> tmp;
		v2 next = keyLookup[c];
		v2 d = next - pos;
		permutator(keys, tmp, d, pos);
		pos = next;
		flatenator(ret, tmp);
	}
}

int repeats(dirStr &s) {
	int ret = 0;
	for (int i = 1; i < s.size(); i++) {
		if (s[i - 1] == s[i])
			ret ++;
	}
	return ret;
}

void robot(dirStr &dirs, int &res, int depth) {
	vector<dirStr> tr;
	translator(tr, directional, dirs, *strtDir);
	int prune = 0;
	depth --;
	for (auto &t : tr) {
		if (res && t.size() > res)
			continue;
		if (depth > 0) {
			if (prune > repeats(t))
				continue;
			if (prune < repeats(t))
				prune = repeats(t);
				robot(t, res, depth);
		} else {
			if (!res || res > t.size()) {
				res = t.size();
				string s;
				for (auto &c : t)
					s += dirCh[c];
				cout << "curr best: " << res << " " << s << endl;
			}
		}
	}
}

int robot(string code) {
	int ret = 0;
	vector<dirStr> tr;
	translator(tr, numeric, code, *strtNum);
	int prune = 0;
	for (auto &t : tr) {
		if (prune > repeats(t))
			continue;
		if (prune < repeats(t))
			prune = repeats(t);
		robot(t, ret, 2);
	}
	return ret;
}

int partOne(vector<string> &codes) {
	int ret = 0;
	cout << endl;
	strtNum = new v2();
	strtDir = new v2();
	*strtNum = findKey(numeric, 'A');
	*strtDir = findKey(directional, 'A');
	if (!keyLookup) {
		keyLookup = new v2[eMax]();
		for (int i = 0; i != eMax; i++) {
			keyLookup[i] = findKey(directional, dirCh[i]);
		}
	}
	for (auto &code : codes) {
		int shortest = robot(code);
		int digits;
		sscanf(code.c_str(), "%dA", &digits);
		cout << digits << " * " << shortest << endl;
		ret += digits * shortest;
	}
	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");
	vector<string> in;

	for (string s; ifs >> s;)
		in.push_back(s);

	cout << "Part One: " << partOne(in) << endl;
	return 0;
}
