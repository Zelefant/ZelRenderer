#ifndef BSP_GENERATOR_CLASS_H
#define BSP_GENERATOR_CLASS_H

#include <string>
#include <vector>
#include "BSPNode.h"
#include "BSPVertex.h"
#include "MapGeometry.h"

enum class Side
{
	Coplanar,
	Front,
	Back,
	Spanning
};

class BSPTree
{
public:
	BSPTree(std::string map_file_path);

	bool CompileBSPIntoFile(std::string compiled_file_path);

private:

	std::random_device rd;
	std::mt19937 g;

	int nextVertexID = 100000;
	int nextLineID = 100000;

	MapGeometry LoadMapGeometryFromFile(std::string file_path);

	BSPNode* GenerateBSP(std::vector<Linedef*>& linedefs, int heuristic = 5);

	BSPVertex* Intersect(Linedef* line, Linedef* splitter);

	void SplitLine(
		Linedef* line,
		Linedef* splitter,
		std::vector<Linedef*>& front,
		std::vector<Linedef*>& back);

	Side CheckCross(Linedef* line, Linedef* check);

	BSPNode* root;
};


#endif