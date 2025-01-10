#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int partOne(vector<string> &input) {
	map<string, vector<string>> graph; 
	for (auto &in : input) {
		string f = in.substr(0, 2);
		string s = in.substr(3, 2);

		if (!graph.count(f))
			graph[f] = vector<string>();
		if (!graph.count(s))
			graph[s] = vector<string>();
		
		graph[f].push_back(s);
		graph[s].push_back(f);
	}

	vector<string> triplets;

	for (auto &[k, v] : graph) {
		for (auto &n : v) {
			auto &nv = graph[n];
			for (auto &nn : nv) {
				auto &nnv = graph[nn];
				if (find(begin(nnv), end(nnv), k) != end(nnv)) {
					vector<string> tr({ k, n, nn });
					sort(begin(tr), end(tr));
					triplets.push_back(tr[0] + tr[1] + tr[2]);
				}
			}
		}
	}
	sort(begin(triplets), end(triplets));
	triplets.erase(unique(begin(triplets), end(triplets)), end(triplets));

	int ret = 0;
	for (auto &t : triplets)
		if (t[0] == 't' || t[2] == 't' || t[4] == 't')
			ret++;
	return ret;
}

string partTwo(vector<string> &input) {
	map<string, vector<string>> graph; 
	for (auto &in : input) {
		string f = in.substr(0, 2);
		string s = in.substr(3, 2);

		if (!graph.count(f))
			graph[f] = vector<string>();
		if (!graph.count(s))
			graph[s] = vector<string>();
		
		graph[f].push_back(s);
		graph[s].push_back(f);
	}

	for (auto &[k, v] : graph)
		sort(begin(v), end(v));

	auto related = [&graph] (string &tgt, string &next, vector<string> &set) -> bool {
		auto &nbrs = graph[next];
		if (find(begin(nbrs), end(nbrs), tgt) == end(nbrs))
			return false;
		for (auto &e : set) {
			if (find(begin(nbrs), end(nbrs), e) == end(nbrs))
				return false;
		}
		return true;
	};

	string ret;
	for (auto &[k, v] : graph) {
		for (int i = 0; i < v.size(); i++) {
			vector<string> rel({ k, v[i] });
			string tmp = k + "," + v[i];
			for (int j = 0; j < v.size(); j++) {
				if (i == j)
					continue;
				if (related(v[i], v[j], rel)) {
					rel.push_back(v[j]);
					tmp += "," + v[j];
				}
			}
			if (tmp.size() > ret.size())
				ret = tmp;
		}
	}

	return ret;
}

int main(int argc, char **argv) {
	ifstream ifs(argc == 2 ? argv[1] : "input");

	vector<string> input;
	for (string s; ifs >> s;)
		input.push_back(s);

	cout << "Part One: " << partOne(input) << endl;
	cout << "Part Two: " << partTwo(input) << endl;
	return 0;
}
