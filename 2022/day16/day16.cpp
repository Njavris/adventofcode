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
		int dst;
		int nxt;
		int hops;
		std::string nm;
		std::string next;
	};
	struct valve {
		int id;
		std::string nm;
		int flow;
		std::vector<std::string> ngbr;
		std::vector<int> ngbrIds;
		std::vector<struct route> routes;
		const bool inline operator==(std::string othr) const { return nm == othr; };
	};
//	std::map<std::string, struct valve> valves;
	std::vector<struct valve> valves;
	void calcRoutes(int pass) {
		for (int i = 0; i < valves.size(); i++) {
			struct valve *v = &valves[i];
			for (auto n: v->ngbrIds) {
				struct valve *ngbr = &valves[n];
				if (!pass && v->routes[n].dst < 0) {
					v->routes[n] = {ngbr->id, ngbr->id, 1, ngbr->nm, ngbr->nm};
					continue;
				}
				for (int j = 0; j < ngbr->routes.size(); j++) {
					struct route *r = &ngbr->routes[j];
					if (r->dst < 0 || r->dst == v->id)
						continue;

					if (v->routes[r->dst].dst < 0 /*||
							v->routes[r->dst].hops + 1 > r->hops*/)
						v->routes[r->dst] = {r->dst, ngbr->id, r->hops + 1,
							r->nm, ngbr->nm};
				}
			}
		}
	};
	void printRoutes(void) {
		for (auto v = valves.begin(); v != valves.end(); v++) {
			std::cout << v->id << "(" << v->nm << ") routes: ";
			for (auto r = v->routes.begin(); r != v->routes.end(); r++) {
				if (r->dst < 0)
					continue;
				std::cout << r->nm << "(" << r->dst << "," << r->nxt << "," <<
					r->next << "," << r->hops << ") ";
			}
			std::cout << std::endl;
		}
	};
	int calcPressure(std::vector<int> path, int time = 26) {
		int pressure = 0;
		int curr = 0;
		for (auto n: path) {
			int hops = valves[curr].routes[n].hops;
			if (time < 0)
				return -1;
			if (time > hops)
				pressure += (time - hops - 1) * valves[n].flow;
			time -= hops + 1;
			curr = n;
		};
		return pressure;
	};
	int findBestPartOne(std::vector<int> &curr, std::vector<int> &set) {
		int best = calcPressure(curr, 30);
		if (best < 0)
			return -1;
		for (int i = 0; i < set.size(); i++) {
			if (std::find(begin(curr), end(curr), set[i]) != curr.end())
				continue;
			curr.push_back(set[i]);
			int tmp = findBestPartOne(curr, set);
			if (tmp > best)
				best = tmp;
			curr.pop_back();
		}
		return best;
	}
	void doPartOne(void) {
		std::vector<int> dsts;
		std::cout << "Part One start!" << std::endl;
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < valves.size(); i++)
			calcRoutes(i);
//		printRoutes();
		for (int i = 0; i < valves.size(); i++) {
			if (valves[i].flow)
				dsts.push_back(valves[i].id);
		}
//		std::sort(begin(dsts), end(dsts),
//				[this](int &lhs, int &rhs){
//					return valves[lhs].flow > valves[rhs].flow;
//				});

		std::cout << "dsts: " << dsts.size() << std::endl;
		std::vector<int> path;
		int bestPressure = findBestPartOne(path, dsts);

		auto done = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(done - start);
		std::cout << "Time taken by partOne: " << duration.count() << " ms" << std::endl;
		if (bestPressure != 1460) {
			std::cout << "Wrong!" << std::endl;
		}
		partOneAnswer = bestPressure;

	};
	void doPartTwo(void) {
		partTwoAnswer = 0;
	};
public:
	Cave(std::string filename) {
		int valveId = 0;
		Input in(filename.size() ? filename : "input");
		while (in.nextLine()) {
			std::string line = in.getLine();
			std::string s;
			valves.push_back({});
			struct valve *v = &valves.back();
			v->nm = std::string(2, '\0');
			v->id = valveId++;
			sscanf(line.c_str(), "Valve %2s has flow rate=%d; tunnels lead to",
				const_cast<char *>(v->nm.c_str()), &v->flow);

			int off = line.find("valve ");
			if (off != std::string::npos)
				off += strlen("valve ");
			else
				off = line.find("valves ") + strlen("valves ");

			for (int i = off, prev = off; i < line.size() + 1; i++) {
				if (line[i] == ',' || line[i] == '\0')
					v->ngbr.push_back(line.substr(prev, i - prev));
				if (line[i] == ' ' || line[i] == ',')
					prev = i + 1;
			};

			std::cout << line << std::endl;
		}
		for (auto v = valves.begin(); v != valves.end(); v++) {
			v->routes.resize(valves.size(), {-1, -1, -1, "", ""});
			for (auto n: v->ngbr) {
				for (int i = 0; i < valves.size(); i++) {
					if (valves[i].nm == n) {
						v->ngbrIds.push_back(i);
						break;
					}
				}
			}
		}
		for (auto v = valves.begin(); v != valves.end(); v++) {
			std::cout << v->id << ": " << v->nm << " " << v->flow << " ";
			for (int n = 0; n < v->ngbr.size(); n++)
				std::cout << v->ngbr[n] << "(" << v->ngbrIds[n] << ") ";
			std::cout << std::endl;
		}
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
