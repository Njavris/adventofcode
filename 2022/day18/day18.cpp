#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cstring>

#include "utils.h"

class Shape {
	int partOneVal;
	int partTwoVal;
	int xMin, yMin, zMin;
	int xMax, yMax, zMax;
	int width, height, depth;
	int *voxelSpace;
	struct voxel {
		int x;
		int y;
		int z;
		bool operator== (struct voxel &p) const {
			return p.x == x && p.y == y && p.z == z;
		};
		std::string toStr(void) {
			return std::to_string(x) + "," +
				std::to_string(y) + "," +
				std::to_string(z);
		};
		bool adjacent(struct voxel &p) const {
			if (*this == p)
				return false;
			if ((p.x == x && p.y == y && (p.z == (z + 1) || p.z == (z - 1))) ||
				(p.x == x && p.z == z && (p.y == (y + 1) || p.y == (y - 1))) ||
				(p.y == y && p.z == z && (p.x == (x + 1) || p.x == (x - 1))))
				return true;
			return false;
		};
		void getAdjacent(std::vector<struct voxel> &adj, int width, int height, int depth) const {
			if (x > 0) adj.push_back({x - 1, y, z});
			if (x < width - 1) adj.push_back({x + 1, y, z});
			if (y > 0) adj.push_back({x, y - 1, z});
			if (y < height - 1) adj.push_back({x, y + 1, z});
			if (z > 0) adj.push_back({x, y, z - 1});
			if (z < depth - 1) adj.push_back({x, y, z + 1});
		};
	};
	std::vector<struct voxel> input;
	int findSurfaceFaces(std::vector<struct voxel> &voxels, struct voxel &v) {
		int ret = 6;
		for (auto i: voxels) {
			if (i == v) continue;
			if (v.adjacent(i)) ret--;
		}
		return ret;
	};
	void doPartOne(void) {
		Timer tim = Timer();
		partOneVal = 0;

		for (auto v: input)
			partOneVal += findSurfaceFaces(input, v);

		tim.stopTiming();
		std::cout << "Part One took: " << tim.getTimeStr() << std::endl;
	};
	inline struct voxel idxToVox(int idx) {
		int x = idx / (height * depth);
		int y = (idx - x * height * depth) / depth;
		int z = idx - x * height * depth - y * depth;
		return {x, y, z};
	};
	inline int voxToIdx(const struct voxel &v) {
		return v.x * height * depth + v.y * depth + v.z;
	};
	void doPartTwo(void) {
		Timer tim = Timer();
		int sz = width * height * depth;
		voxelSpace = new int[sz];
		for (int i = 0; i < sz; i++)
			voxelSpace[i] = 0;
		for (auto v: input)
			voxelSpace[voxToIdx(v)] = -1;

		for (int x = 0; x < width; x++) {
			int idx = voxToIdx({x, 0, 0});
			if (!voxelSpace[idx]) {
				voxelSpace[idx] = 1;
				break;
			}
		}
		int *before = new int[sz];
		int *after = new int[sz];
		for (int i = 0; i < sz; i++) before[i] = 0;
		for (int i = 0; i < sz; i++)
			after[i] = voxelSpace[i] > 0 ? 1 : voxelSpace[i];

		while ([before, after, sz](void) -> bool {
					for (int i = 0; i < sz; i++)
						if (before[i] != after[i])
							return true;
					return false;
				}()) {
			int *tmp = before;
			before = after;
			after = tmp;

			for (int i = 0; i < sz; i++) {
				int max = 0;
				struct voxel vx = idxToVox(i);
				std::vector<struct voxel> vxs;
				if (voxelSpace[i] == -1)
					continue;
				vx.getAdjacent(vxs, width, height, depth);
				for (auto v: vxs)
					if (voxelSpace[voxToIdx(v)] > max)
						max = voxelSpace[voxToIdx(v)];
				if (max > 0)
					voxelSpace[i] = max + 1;
			}
			for (int i = 0; i < sz; i++)
				after[i] = voxelSpace[i] > 0 ? 1 : voxelSpace[i];
		}
		delete before;
		delete after;

		std::vector<struct voxel> trapped;
		for (int i = 0; i < sz; i++) {
			if (!voxelSpace[i])
				trapped.push_back(idxToVox(i));
		}

		partTwoVal = 0;
		for (auto v: input)
			partTwoVal += findSurfaceFaces(input, v);
		for (auto v: trapped)
			partTwoVal -= findSurfaceFaces(trapped, v);

		delete voxelSpace;
		tim.stopTiming();
		std::cout << "Part Two took: " << tim.getTimeStr() << std::endl;
	};
public:
	Shape(std::string fileName) {
		Input in = Input(fileName.size() ? fileName : "input");
		xMin = yMin = zMin = -1;
		xMax = yMax = zMax = 0;
		while (in.nextLine()) {
			int x, y, z;
			std::string ln = in.getLine();
			sscanf(ln.c_str(), "%d,%d,%d", &x, &y, &z);
			input.push_back({++x, ++y, ++z});
			if (xMin < 0 || xMin > x)
				xMin = x;
			if (yMin < 0 || yMin > y)
				yMin = y;
			if (zMin < 0 || zMin > z)
				zMin = z;
			if (xMax < x)
				xMax = x;
			if (yMax < y)
				yMax = y;
			if (zMax < z)
				zMax = z;
		}
		width = xMax + 2;
		height = yMax + 2;
		depth = zMax + 2;
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

int main(int argc, char **argv) {
	std::string param;
	if (argc > 1)
		param = std::string(argv[1]);

	Shape s(param);
	std::cout << "Part One: " << s.partOne() << std::endl;
	std::cout << "Part Two: " << s.partTwo() << std::endl;
	return 0;
}
