#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <map>
#include <chrono>

class Input {
	std::ifstream ifs;
	std::string lastLine;
public:
	Input(std::string filename) : lastLine("") { ifs = std::ifstream(filename); };
	int64_t nextLine(void) { return !!std::getline(ifs, lastLine); };
	std::string getLine(void) { return lastLine; };
};

class Cave {
	int partOneAnswer;
	int partTwoAnswer;
	struct route {
		std::string nm;
		std::string next;
		int hops;
	};
	struct valve {
		std::string nm;
		int flow;
		std::vector<std::string> ngbr;
		std::map<std::string, struct route> routes;
		const bool inline operator==(std::string othr) const { return nm == othr; };
	};
	std::map<std::string, struct valve> valves;
	void calcRoutes(int pass) {
		for (auto it = valves.begin(); it != valves.end(); it++) {
			struct valve *v = &it->second;
			for (auto jt = v->ngbr.begin(); jt != v->ngbr.end(); jt++) {
				struct valve *ngbr = &valves[*jt];
				if (!pass && v->routes.find(*jt) == v->routes.end()) {
					v->routes.insert({*jt, {*jt, *jt, 1}});
					continue;
				}
				for (auto kt = ngbr->routes.begin(); kt != ngbr->routes.end(); kt++) {
					struct route *r = &kt->second;
					if (ngbr->nm == r->nm)
						continue;
					if (v->routes.find(r->nm) == v->routes.end()) {
						v->routes.insert({r->nm, {r->nm, ngbr->nm, r->hops + 1}});
						continue;
					}
				}
			}
		}
	};
	void printRoutes(void) {
		for (auto it = valves.begin(); it != valves.end(); it++) {
			struct valve *v = &it->second;
			std::cout << v->nm << " routes: ";
			for (auto jt = v->routes.begin(); jt != v->routes.end(); jt++) {
				struct route *r = &jt->second;
				std::cout << r->nm << "(" << r->next << "," << r->hops << ") ";
			}
			std::cout << std::endl;
		}
	};
	int calcStepPressure(std::string &src, std::string &dst, int time) {
		struct valve *dstV = &valves[dst];
		struct route *r = &valves[src].routes[dst];
		int pressure = (time > r->hops) ? (time - r->hops - 1) * dstV->flow : 0;

//		std::cout << r->nm << " h: " << r->hops << " f: " << dstV->flow <<
//			" p:" << pressure << std::endl;
		return pressure;
	};
	int calcPressure(std::vector<std::string> path, int time = 26) {
		int pressure = 0;
		std::string curr = "AA";
		for (auto n: path) {
			if (time < 0)
				return -1;
			pressure += calcStepPressure(curr, n, time);
			time -= valves[curr].routes[n].hops + 1;
			curr = n;
		};
		return pressure;
	};
	int findBestPartOne(std::vector<std::string> &curr, std::vector<std::string> &set) {
		int best = calcPressure(curr, 30);
		if (best < 0)
			return -1;
		for (auto it = set.begin(); it != set.end(); it++) {
			if (std::find(begin(curr), end(curr), *it) != curr.end())
				continue;
			curr.push_back(*it);
			int tmp = findBestPartOne(curr, set);
			if (tmp > best)
				best = tmp;
			curr.pop_back();
		}
		return best;
	}
	void doPartOne(void) {
		std::vector<std::string> dsts;
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < valves.size(); i++)
			calcRoutes(i);
//		printRoutes();
		for (auto it = valves.begin(); it !=valves.end(); it++) {
			struct valve *v = &it->second;
			if (v->flow)
				dsts.push_back(v->nm);
		}
//		std::sort(begin(dsts), end(dsts),
//				[this](std::string &lhs, std::string &rhs){
//					return valves[lhs].flow > valves[rhs].flow;
//				});

		std::cout << "dsts: " << dsts.size() << std::endl;
		std::vector<std::string> path;
		int bestPressure = findBestPartOne(path, dsts);

		auto done = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(done - start);
		std::cout << "Time taken by partOne: " << duration.count() << " ms" << std::endl;
		partOneAnswer = bestPressure;

	};
	void doPartTwo(void) {
		partTwoAnswer = 0;
	};
public:
	Cave(std::string filename) {
		Input in(filename.size() ? filename : "input");
		while (in.nextLine()) {
			std::string line = in.getLine();
			std::string s;
//			valves.push_back({});
			struct valve v;
			v.nm = std::string(2, '\0');
			sscanf(line.c_str(), "Valve %2s has flow rate=%d; tunnels lead to",
				const_cast<char *>(v.nm.c_str()), &v.flow);

			int off = line.find("valve ");
			if (off != std::string::npos)
				off += strlen("valve ");
			else
				off = line.find("valves ") + strlen("valves ");

			for (int i = off, prev = off; i < line.size() + 1; i++) {
				if (line[i] == ',' || line[i] == '\0')
					v.ngbr.push_back(line.substr(prev, i - prev));
				if (line[i] == ' ' || line[i] == ',')
					prev = i + 1;
			};
			valves[v.nm] = v;

			std::cout << line << std::endl;
		}
//		for (auto it = valves.begin(); it != valves.end(); it++) {
//			std::cout << it->second.nm << " " << it->second.flow << " ";
//			for (auto s: it->second.ngbr)
//				std::cout << s << " ";
//			std::cout << std::endl;
//		}
		std::cout << valves.size() << std::endl;

		doPartOne();
		doPartTwo();
	};
	int partOne(void) {
		return partOneAnswer;
	};
	int partTwo(void) {
		return partTwoAnswer;
	};
};

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	Cave cave(param);
	std::cout << "Part One: " << cave.partOne() << std::endl;
	std::cout << "Part Two: " << cave.partTwo() << std::endl;
	return 0;
}
