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

class Timer {
	bool stopped;
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
public:
	Timer(void) : stopped(false) { startTiming(); };
	void startTiming(void) {
		stopped = false;
		start = std::chrono::high_resolution_clock::now();
	};
	void stopTiming(void) {
		end = std::chrono::high_resolution_clock::now();
		stopped = true;
	};
	std::string getTimeStr(void) {
		std::string ret = "";
		if (!stopped)
			return "";
		auto durationUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		auto durationS = std::chrono::duration_cast<std::chrono::seconds>(end - start);
		auto durationM = std::chrono::duration_cast<std::chrono::minutes>(end - start);
		auto durationH = std::chrono::duration_cast<std::chrono::hours>(end - start);
		std::chrono::duration<long, std::micro> intUs = durationUs;
		std::chrono::duration<long, std::milli> intMs = durationMs;
		std::chrono::duration<long, std::ratio<1>> intS = durationS;
		std::chrono::duration<long, std::ratio<60>> intM = durationM;
		std::chrono::duration<long, std::ratio<3600>> intH = durationH;

		if (intMs.count()) {
			if (intS.count()) {
				if (intM.count()) {
					if (intH.count()) {
						ret += std::to_string(intH.count()) + "h ";
					}
					ret += std::to_string(intM.count() % 60) + "m ";
				}
				ret += std::to_string(intS.count() % 60) + "s ";
			}
			ret += std::to_string(intMs.count() % 1000) + "ms ";
		}
		ret += std::to_string(intUs.count() % 1000) + "us ";
		ret += "(" + std::to_string(intUs.count()) + "us total)";
		return ret;
	};
};

class Cave {
	int partOneAnswer;
	int partTwoAnswer;
	int currBest;
	int iterationsChecked;
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
	int start;
	std::string bestStr0;
	std::string bestStr1;
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
					if (r->dst == -1 || r->dst == ngbr->id)
						continue;

					if (v->routes[r->dst].dst < 0 ||
							v->routes[r->dst].hops > r->hops + 1) {
						v->routes[r->dst].dst = r->dst;
						v->routes[r->dst].nxt = ngbr->id;
						v->routes[r->dst].hops = r->hops + 1;
						v->routes[r->dst].nm = r->nm;
						v->routes[r->dst].next = ngbr->nm;
					}
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
	void doRoutes(void) {
		Timer timer = Timer();
		for (int i = 0; i < valves.size(); i++)
			calcRoutes(i);
//		printRoutes();
		timer.stopTiming();
		std::cout << "Time taken to calculate routes: " << timer.getTimeStr() << std::endl;
	};
	std::string getPathStr(std::vector<int> path) {
		std::string ret = "";
		for (auto s: path) {
			ret += valves[s].nm + "(";
			ret += std::to_string(s) + ") ";
		}
		return ret;
	};
	int calcPressure(std::vector<int> &path, int time = 30, int off = 0, int end = 0) {
		int pressure = 0;
		int curr = start;
		for (int i = off; i < (end ? end : path.size()); i++) {
			int n = path[i];
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
			if (tmp > best) {
				best = tmp;
				if (best > currBest) {
					currBest = best;
					bestStr0 = getPathStr(curr);
					std::cout << "\u001b[2K" << "Curr best: " << currBest <<
						" steps: " << curr.size() << " " << bestStr0 <<
						std::endl << "\033[F";
				}
			}
			curr.pop_back();
		}
		return best;
	}
	void doPartOne(void) {
		std::vector<int> dsts;
		Timer timer = Timer();
		currBest = 0;

		for (int i = 0; i < valves.size(); i++) {
			if (valves[i].flow)
				dsts.push_back(valves[i].id);
		}
		std::vector<int> path;
		int bestPressure = findBestPartOne(path, dsts);

		timer.stopTiming();
		std::cout << std::endl << "Time taken by part one: " << timer.getTimeStr() << std::endl;
		partOneAnswer = bestPressure;
	};
	int findBestPartTwoSecond(std::vector<int> &path0, int path0Pressure, std::vector<int> &curr,
			std::vector<int> &set) {
		int best = calcPressure(curr, 26);
		if (best < 0)
			return -1;
		for (int i = 0; i < set.size(); i++) {
			if ((std::find(begin(curr), end(curr), set[i]) != curr.end()) || 
					(std::find(begin(path0), end(path0), set[i]) != path0.end()))
				continue;
			curr.push_back(set[i]);
			int tmp = findBestPartTwoSecond(path0, path0Pressure, curr, set);
			if (tmp > best) {
				best = tmp;
				if (best + path0Pressure > currBest) {
					currBest = best + path0Pressure;
					bestStr0 = getPathStr(path0);
					bestStr1 = getPathStr(curr);
					std::cout << "\u001b[2K" << "Curr best: " <<
						currBest << " steps: " << curr.size() << " " <<
						bestStr0 << "| " << bestStr1 << std::endl << "\033[F";
				}
			}
			curr.pop_back();
		}
		return best;
	};
	int findBestPartTwo(std::vector<int> &curr, std::vector<int> &set) {
		int path0 = calcPressure(curr, 26);
		if (path0 < 0)
			return -1;
		std::vector<int> p1;
		int path1 = findBestPartTwoSecond(curr, path0, p1, set);
		if (path1 < 0)
			return -1;
		int best = path0 + path1;

		for (int i = 0; i < set.size(); i++) {
			if (std::find(begin(curr), end(curr), set[i]) != curr.end())
				continue;
			curr.push_back(set[i]);
			int tmp = findBestPartTwo(curr, set);
			if (tmp > best) {
				best = tmp;
			}
			curr.pop_back();
		}
		return best;
	}
	void doPartTwo(void) {
		std::vector<int> dsts;
		Timer timer = Timer();
		currBest = 0;

		for (int i = 0; i < valves.size(); i++) {
			if (valves[i].flow)
				dsts.push_back(valves[i].id);
		}
		std::vector<int> path;
		int bestPressure = findBestPartTwo(path, dsts);

		timer.stopTiming();
		std::cout << std::endl << "Time taken by part two: " << timer.getTimeStr() << std::endl;
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

//			std::cout << line << std::endl;
		}
		for (auto v = valves.begin(); v != valves.end(); v++) {
			if (v->nm == "AA")
				start = v->id;
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
