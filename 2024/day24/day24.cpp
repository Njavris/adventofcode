#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <format>

using namespace std;

int calc(map<string, unsigned> &vals, map<string, vector<string>> &ops, const string &key, int depth = 0) {
	if (depth > ops.size())
		return -1;
	if (vals.count(key))
		return vals[key];

	int val;
	if (!ops.count(key))
		return -1;

	auto &op = ops[key];

	if (!vals.count(op[0]))
		calc(vals, ops, op[0], depth + 1);
	unsigned &o1 = vals[op[0]];
	if (!vals.count(op[2]))
		calc(vals, ops, op[2], depth + 1);
	unsigned &o2 = vals[op[2]];

	if (op[1] == "OR")
		val = o1 | o2;
	if (op[1] == "AND")
		val = o1 & o2;
	if (op[1] == "XOR")
		val = o1 ^ o2;
	val &= 1;
	vals[key] = val;
	return val;
}

unsigned long partOne(map<string, unsigned> &vals, map<string, vector<string>> &ops) {
	unsigned long ret = 0;
	for (int i = 0;; i++) {
		string k = format("z{:02}", i);
		long int r = calc(vals, ops, k);
		if (r < 0)
			break;
		ret |= (r & 1) << i;
	}
	return ret;
}

unsigned long calcP2(map<string, vector<string>> &ops, unsigned long x, unsigned long y, int bits) {
	unsigned long ret = 0;
	map<string, unsigned> tmp;
	
	for (int i = 0; i < bits; i++) {
		string kX = format("x{:02}", i);
		string kY = format("y{:02}", i);
		tmp[kX] = (x >> i) & 1;
		tmp[kY] = (y >> i) & 1;
	}

	for (int i = 0;; i++) {
		string k = format("z{:02}", i);
		long int r = calc(tmp, ops, k);
		if (r < 0)
			break;
		ret |= (r & 1) << i;
	}
	return ret;
}


string recursivePrint(map<string, vector<string>> &ops, string key, int bit, int depth) {
	string ret = key;
	if (!depth)
		return ret;
	if (ops.count(key)) {
		auto &op = ops[key];
		string o1 = op[0];
		string o2 = op[2];

		if (ops.count(o1) && ops.count(o2)) {
			if (ops[o1][1] == "XOR" && ops[o2][1] == "OR") {
				string t = o1;
				o1 = o2;
				o2 = t;
			}
			if (ops[o1][1] == "OR" && ops[o2][1] == "AND") {
				string t = o1;
				o1 = o2;
				o2 = t;
			}
			if (ops[o1][1] == "XOR" && ops[o2][1] == "AND") {
				string t = o1;
				o1 = o2;
				o2 = t;
			}
		}
		if (ops.count(o1))
			o1 = recursivePrint(ops, o1, bit, depth - 1);
		if (ops.count(o2))
			o2 = recursivePrint(ops, o2, bit, depth - 1);
		ret = format("({}: {} {:3} {})", key, o1, op[1], o2);
	}
		
	return ret;
}

string partTwo(map<string, unsigned> &vals, map<string, vector<string>> &ops) {
	int bits = 0;
	for (;ops.count(format("z{:02}", bits)); bits++);
	auto tests = [&bits, &ops] () -> int {
		int ret = 0;
		for (int i = 0; i < bits; i++) {
			unsigned long a = 1ul << i;
			unsigned long taa = a + a;
			unsigned long raa = calcP2(ops, a, a, bits);
			unsigned long ran = calcP2(ops, a, 0ul, bits);
			unsigned long rna = calcP2(ops, 0ul, a, bits);
			if ((taa != raa) || (a != ran || a != rna))
				ret ++;
		}
		return ret;
	};

	vector<string> rets;
	auto swapOps = [&ops, &rets] (string f, string s) {
		auto tmp1 = ops.extract(f);
		auto tmp2 = ops.extract(s);
		tmp1.key() = s;
		tmp2.key() = f;
		ops.insert(std::move(tmp2));
		ops.insert(std::move(tmp1));
		rets.push_back(f);
		rets.push_back(s);
	};
	swapOps("z30", "z30");

/*
	for (int i = 0; i < bits; i++) {
		string out = format("z{:02}", i);
		auto &op = ops[out];
		string pattern = format("(A {} B)", op[1]);
		pattern = recursivePrint(ops, out, i, 2);
		cout << out << " = " << pattern << endl;
	}
	cout << "Wrong bits: " << tests() << endl;
*/


	string ret;
	sort(begin(rets), end(rets));
	for (int i = 0; i < rets.size(); i++)
		ret += (i ? "," : "") + rets[i];

	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");

	map<string, unsigned> vals;
	map<string, vector<string>> ops;

	for (string s; getline(ifs, s);) {
		if (!s.size())
			break;
		string k = s.substr(0, s.find(':'));
		string v = s.substr(s.find(':') + 1);
		vals[k] = stoul(v);
	}

	for (string s; getline(ifs, s);) {
		istringstream ss(s);
		string k, o1, o2, op;
		ss >> o1 >> op >> o2 >> k >> k;
		ops[k] = vector<string>({ o1, op, o2, k });
	}

	cout << "Part One: " << partOne(vals, ops) << endl;
	cout << "Part Two: " << partTwo(vals, ops) << endl;
	return 0;
}
