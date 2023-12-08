#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>

using namespace std;

struct node {
	string name, left, right;
	int idxL, idxR;
	node(string &nm, string &left, string &right) :
			name(nm), left(left), right(right), idxL(-1), idxR(-1) {};
};

int solveP1(vector<node> &nodes, string &instr, int start, int end) {
	int curr = start, steps = 0;
	while (curr != end) {
		node &n = nodes[curr];
		char p = instr[steps % instr.length()];
		curr = (p == 'L') ? n.idxL : (p == 'R') ? n.idxR : -1;
		steps ++;
	}
	return steps;
}

int64_t solveP2(vector<node> &nodes, string &instr, vector<int> curr) {
	int64_t ret = 1;
	int steps = 0;
	vector<int> res;
	res.resize(curr.size(), 0);
	while (true) {
		char p = instr[steps % instr.length()];
		bool cont = false;
		for (int i = 0; i < curr.size(); i++) {
			if (res[i])
				continue;
			int &c = curr[i];
			node &n = nodes[c];
			c = (p == 'L') ? n.idxL : (p == 'R') ? n.idxR : -1;
			if (n.name[2] == 'Z')
				res[i] = steps;
			cont = true;
		}
		if (!cont)
			break;
		steps ++;
	}
	for (auto &r: res)
		ret = lcm(ret, r);
	return ret;
}

int main(int c, char **v) {
	if (c != 2)
		return -1;

	ifstream ifs(v[1]);
	string line, instr;
	getline(ifs, instr);

	vector<node> nodes;
	while (getline(ifs, line)) {
		if (!line.length())
			continue;
		string name = line.substr(0, 3);
		string left = line.substr(line.find("(") + 1, 3);
		string right = line.substr(line.find(", ") + 2, 3);
		nodes.push_back(node(name, left, right));
	}
	int start, end;
	vector<int> starts;
	for (int i = 0; i < nodes.size(); i++) {
		node &n = nodes[i];
		if (n.name == "AAA")
			start = i;
		if (n.name == "ZZZ")
			end = i;
		if (n.name[2] == 'A')
			starts.push_back(i);
		for (int j = 0; j < nodes.size(); j++) {
			node &nj = nodes[j];
			if (nj.name == n.left)
				n.idxL = j;
			if (nj.name == n.right)
				n.idxR = j;
		}
	}
	cout << "Part One: " << solveP1(nodes, instr, start, end) << endl;
	cout << "Part Two: " << solveP2(nodes, instr, starts) << endl;
	return 0;
}
