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
	std::vector<struct valve> valves;
	int startId;
	std::vector<int> dsts;
	int bestVal;
	std::vector<int> bestPath;
	void calcRoutes(int pass) {
		for (int i = 0; i < valves.size(); i++) {
			struct valve *v = &valves[i];
			for (auto n: v->ngbrIds) {
				struct valve *ngbr = &valves[n];
				if (!pass && v->routes[n].dst == -1) {
					v->routes[n] = {ngbr->id, ngbr->id, 1, ngbr->nm, ngbr->nm};
					continue;
				}
				for (int j = 0; j < ngbr->routes.size(); j++) {
					struct route *r = &ngbr->routes[j];
					if (r->dst == -1 || r->dst == ngbr->id || r->dst == v->id)
						continue;

					if (v->routes[r->dst].dst < 0 ||
							v->routes[r->dst].hops > r->hops + 1) {
						v->routes[r->dst] = { r->dst, ngbr->id, r->hops + 1, r->nm, ngbr->nm} ;
					}
				}
			}
		}
	};
	void printRoutes(void) {
		for (auto v = valves.begin(); v != valves.end(); v++) {
			std::cout << v->id << "(" << v->nm << ") routes: ";
			for (auto r = v->routes.begin(); r != v->routes.end(); r++) {
//				if (r->dst < 0)
//					continue;
				std::cout << r->nm << "(" << r->dst << "," << r->nxt << "," <<
					r->next << "," << r->hops << ") ";
			}
			std::cout << std::endl;
		}
	};
	void doRoutes(void) {
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < valves.size(); i++)
			calcRoutes(i);
//		printRoutes();
		for (int i = 0; i < valves.size(); i++) {
			if (valves[i].flow)
				dsts.push_back(valves[i].id);
		}
		auto done = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(done - start);
		std::cout << "Time taken by route finding and sorting: " <<
						duration.count() << " ms" << std::endl;
		std::cout << "Non zero destinations: " << dsts.size() << std::endl;
	}
	inline int calcPressure(std::vector<int> path, int start, int time, int off = 0, int limit = 0) {
		int pressure = 0;
		int curr = start;
		if (limit < off)
			return -1;
		std::cout << off << "-" << limit << " ";
		for (int i = off; i < (limit ? limit : path.size()); i++) {
			int n = path[i];
			struct route *r = &valves[curr].routes[n];
			int hops = r->hops;
			std::cout << valves[n].nm << " ";
			time -= hops + 1;
			if (time > 0) {
				pressure += time * valves[n].flow;
			} else {
				pressure = -1;
				break;
			}
			curr = n;
		}
		std::cout << pressure << std::endl;
		return pressure;
	};
	int findBestPartOne(std::vector<int> &curr, std::vector<int> &set, int startId) {
		int best = calcPressure(curr, startId, 30);
		if (best < 0)
			return -1;
		if (best > bestVal) {
			bestPath = curr;
			bestVal = best;
		}
		for (int i = 0; i < set.size(); i++) {
			if (std::find(begin(curr), end(curr), set[i]) != curr.end())
				continue;
			curr.push_back(set[i]);
			int tmp = findBestPartOne(curr, set, startId);
			if (tmp > best)
				best = tmp;
			curr.pop_back();
		}
		return best;
	};
	void doPartOne(void) {
		int bestPressure;
		std::vector<int> open;
		auto start = std::chrono::high_resolution_clock::now();
		bestVal = 0;

		std::vector<int> path;
//		bestPressure = findBestPartOne(path, dsts, startId);

		auto done = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(done - start);
		std::cout << "Time taken by Part One: " << duration.count() << " ms" << std::endl;
		for (auto s: bestPath)
			std::cout << valves[s].nm << " ";
		std::cout << std::endl;
		partOneAnswer = bestPressure;
	};
//	inline int calcPressureTwo(std::vector<int> path, int start, int time) {
//		int best = 0;
//		if (path.size() < 2)
//			return 0;
//		for (int i = 1; i < path.size() - 1; i++) {
//			int press[2];
//			press[0] = calcPressure(path, start, time, 0, i);
//			if (press[0] < 0)
//				continue;
//			press[1] = calcPressure(path, start, time, i, path.size());
//			if (press[1] < 0)
//				continue;
//			int sum = press[0] + press[1];
//			if (sum > best)
//				best = sum;
//		}
//		return best;
//	};
//	int calcBestPartTwo(std::vector<int> &curr, std::vector<int> &set, int startId, int off) {
//		int best = 0;
//		int press[2];
//		press[0] = calcPressure(curr, startId, 26, 0, off);
//		if (press[0] < 0)
//			return -1;
//
//		for (int j = 0; j < set.size(); j++) {
//			if (std::find(begin(curr), end(curr), set[j]) != curr.end())
//				continue;
//			curr.push_back(set[j]);
//			press[1] = calcPressure(curr, startId, 26, j, curr.size());
//			if (press[1] < 0)
//				continue;
//			int total = press[0] + press[1];
//			if (total > best) {
//				best = total;
//				if (best > bestVal) {
//					bestPath = curr;
//					bestVal = best;
//					std::cout << /*"\033[F\33[2K" <<*/ "Currently best: ";
//					for (auto s: bestPath)
//						std::cout << valves[s].nm << " ";
//					std::cout << bestVal << std::endl;
//				}
//			}
//		}
//
//		return best;
//	}
	int findBestPartTwo(std::vector<int> &curr, std::vector<int> &set, int startId, int depth) {
		int best = 0;
		for (int i = 0; i < set.size(); i++) {
			if (std::find(begin(curr), end(curr), set[i]) != curr.end())
				continue;
			curr.push_back(set[i]);
			int tmp = findBestPartTwo(curr, set, startId), depth + 1;
			if (tmp > best)
				best = tmp;
			curr.pop_back();
		}
		return best;
	};
	void doPartTwo(void) {
		int bestPressure;
		bestPath.erase(begin(bestPath), end(bestPath));
		auto start = std::chrono::high_resolution_clock::now();
		bestVal = 0;

		std::vector<int> path;
		std::cout << std::endl;
		bestPressure = findBestPartTwo(path, dsts, startId, 0);
		std::cout << std::endl;

		auto done = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(done - start);
		std::cout << "Time taken by Part Two: " << duration.count() << " ms" << std::endl;
		for (auto s: bestPath)
			std::cout << valves[s].nm << " ";
		std::cout << std::endl;
		partTwoAnswer = bestPressure;
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

			if (v->nm == "AA")
				startId = v->id;
//			std::cout << line << std::endl;
		}
		for (auto v = valves.begin(); v != valves.end(); v++) {
			v->routes.resize(valves.size(), {-1, -1, -1, "", ""});
			for (auto nNm: v->ngbr) {
				auto n = std::find_if(begin(valves), end(valves),
					[nNm](const auto &o) { return o.nm == nNm; });
				v->ngbrIds.push_back(n->id);
			}
		}
//		for (auto v = valves.begin(); v != valves.end(); v++) {
//			std::cout << v->id << ": " << v->nm << " " << v->flow << " ";
//			for (int n = 0; n < v->ngbr.size(); n++)
//				std::cout << v->ngbr[n] << "(" << v->ngbrIds[n] << ") ";
//			std::cout << std::endl;
//		}
//		std::cout << valves.size() << std::endl;

		doRoutes();
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
