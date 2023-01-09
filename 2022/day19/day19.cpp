#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cstring>

#include "utils.h"


class CollectingRobots {
	int partOneVal;
	int partTwoVal;
	enum {
		eOre = 0,
		eClay,
		eObsid,
		eGeode,
		eMatMax
	};
	static constexpr const char *nmMat[eMatMax + 1] = {
		[eOre] = "Ore",
		[eClay] = "Clay",
		[eObsid] = "Obsidian",
		[eGeode] = "Geode",
		[eMatMax] = "",
	};
	enum {
		eStock = 0,
		eRobots,
		eInvMax,
	};
	struct res {
		int mat[eMatMax];
	};
	struct inv {
		struct res res[eInvMax];
	};
	struct blueprint {
		int id;
		struct res robots[eMatMax];
		int maxMat[eMatMax];
	};
	std::vector<struct blueprint> input;
	std::string toStr(const struct res &r) {
		std::string ret;
		for (int i = 0; i < sizeof(r.mat) / sizeof(r.mat[0]); i++) {
			if (r.mat[i])
				ret += std::string(nmMat[i]) + ": " + std::to_string(r.mat[i]) + " ";
		}
		return ret;
	};
	std::string toStr(const struct inv &i) {
		return "Stock: " + toStr(i.res[eStock]) + " Robots: " + toStr(i.res[eRobots]);
	};
	std::string toStr(const struct blueprint &bp) {
		std::string ret = "Blueprint: " + std::to_string(bp.id) + "\n";
		for (int i = 0; i < sizeof(bp.robots) / sizeof(bp.robots[0]); i++)
			ret += " " + std::string(nmMat[i]) + " robot costs: " + toStr(bp.robots[i]) + "\n";
		return ret;
	};
	inline bool build(struct blueprint &bp, struct inv &inv, int r) {
		struct res *rbp = &bp.robots[r];
		if (r == eMatMax) return true; /* build nothing */
		for (int i = eOre; i < eMatMax; i++)
			if (rbp->mat[i] > inv.res[eStock].mat[i])
				return false; /* not enough materials */
		for (int i = eOre; i < eMatMax; i++)
			inv.res[eStock].mat[i] -= rbp->mat[i];
		inv.res[eRobots].mat[r] ++;
		return true;
	};
	int best;
	bool prune(struct blueprint &bp, struct inv &inv, int r, int time, int currBest) {
		struct res *robots = &inv.res[eRobots];
		struct res *stock = &inv.res[eStock];

		/* Can it beat best */
		int gr = robots->mat[eGeode];
		int g = stock->mat[eGeode];
		int t = time;
		while (t > 0) { g += gr; gr ++; t --; }
		if (g < best)
			return true;

		/* always try to build geode and obsidian robots */
		if (r == eGeode)
			return false;

		/* If it can afford geode robot, don't even look at other branches */
		if (stock->mat[eOre] >= bp.robots[eGeode].mat[eOre] &&
				stock->mat[eObsid] >= bp.robots[eGeode].mat[eObsid])
			return true;

		/* if theres enough material robots to build max required, then dont build any more robots */
		if (r < eMatMax && robots->mat[r] >= bp.maxMat[r])
			return true;

		return false;
	};
	int findBestScore(struct blueprint &bp, struct inv inv, int r, int time) {
		int currBest = 0;
		if (!time)
			return inv.res[eStock].mat[eGeode];
		if (!build(bp, inv, r))
			return 0;
		for (int i = eOre; i < eMatMax; i++)
			inv.res[eStock].mat[i] += inv.res[eRobots].mat[i];
		if (r != eMatMax)
			inv.res[eStock].mat[r] --;
		for (int i = eOre; i <= eMatMax; i++) {
			if (prune(bp, inv, i, time - 1, currBest))
				continue;
			int curr = findBestScore(bp, inv, i, time - 1);
			if (curr > currBest)
				currBest = curr;
		}
		if (currBest > best) {
			best = currBest;
//			std::cout << "\u001b[2K" << "Best so far: " << best << " " <<
//				toStr(inv) << std::endl << "\033[F";
		}
		return currBest;
	};
	void doPartOne(void) {
		Timer timer = Timer();
		partOneVal = 0;
		for (auto bi = begin(input); bi != end(input); bi++) {
			best = 0;
//			std::cout << toStr(*bi) << std::endl;
			struct inv inv = { 0 };
			inv.res[eRobots].mat[eOre] = 1;
//			std::cout << "Starting with: " << std::endl << toStr(inv) << std::endl;
			int ret = findBestScore(*bi, inv, eMatMax, 24);
			partOneVal += ret * bi->id;
//			std::cout << std::endl;
//			std::cout << "Best: " << ret << std::endl;
		}
		timer.stopTiming();
		std::cout << std::endl << "Time taken by part One: " << timer.getTimeStr() << std::endl;
	};
	void doPartTwo(void) {
		Timer timer = Timer();
		partTwoVal = 0;
		for (auto bi = begin(input); bi != end(input); bi++) {
			if (bi->id > 3)
				continue;
			best = 0;
//			std::cout << toStr(*bi) << std::endl;
			struct inv inv = { 0 };
			inv.res[eRobots].mat[eOre] = 1;
//			std::cout << "Starting with: " << std::endl << toStr(inv) << std::endl;
			int ret = findBestScore(*bi, inv, eMatMax, 32);
			partTwoVal = !partTwoVal ? ret : partTwoVal * ret;
//			std::cout << std::endl;
//			std::cout << "Best: " << ret << std::endl;
		}
		timer.stopTiming();
		std::cout << std::endl << "Time taken by part Two: " << timer.getTimeStr() << std::endl;
	};
public:
	CollectingRobots(std::string fileName) {
		Input in = Input(fileName.size() ? fileName : "input");
		while (in.nextLine()) {
			std::string ln = in.getLine();
//			std::cout << ln << std::endl;
			struct blueprint bp { 0 };
			sscanf(ln.c_str(), "Blueprint %d: Each ore robot costs %d ore. "
				"Each clay robot costs %d ore. "
				"Each obsidian robot costs %d ore and %d clay. "
				"Each geode robot costs %d ore and %d obsidian.",
				&bp.id,
				&bp.robots[eOre].mat[eOre],
				&bp.robots[eClay].mat[eOre],
				&bp.robots[eObsid].mat[eOre],
				&bp.robots[eObsid].mat[eClay],
				&bp.robots[eGeode].mat[eOre],
				&bp.robots[eGeode].mat[eObsid]);

			for (int i = eOre; i < eMatMax; i++) {
				for (int j = eOre; j < eMatMax; j++)
					if (bp.maxMat[i] < bp.robots[j].mat[i])
						bp.maxMat[i] = bp.robots[j].mat[i];
			}
			input.push_back(bp);
		}

		doPartOne();
		doPartTwo();
	};
	int partOne(void) {
		return partOneVal;
	};
	int partTwo(void) {
		return partTwoVal;
	};
};
constexpr const char *CollectingRobots::nmMat[CollectingRobots::eMatMax + 1];

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	CollectingRobots r(param);
	std::cout << "Part One: " << r.partOne() << std::endl;
	std::cout << "Part Two: " << r.partTwo() << std::endl;
	return 0;
}
