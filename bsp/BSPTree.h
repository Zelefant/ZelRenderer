#ifndef BSP_GENERATOR_CLASS_H
#define BSP_GENERATOR_CLASS_H

#include <string>
#include <vector>
#include "BSPNode.h"
#include "BSPVertex.h"
#include "MapGeometry.h"

class BSPTree
{
public:
	BSPTree(std::string map_file_path);

	bool CompileBSPIntoFile(std::string compiled_file_path);

private:

	std::random_device rd;
	std::mt19937 g;

	MapGeometry LoadMapGeometryFromFile(std::string file_path);

	BSPNode* GenerateBSP(std::vector<BSPVertex>& wallList);

	void CreateNewShape(
		std::vector<BSPVertex*>* shape1,
		std::vector<BSPVertex*>* shape2,
		std::vector<BSPVertex*> cross,
		std::vector<BSPVertex*> front,
		std::vector<BSPVertex*> back
	);

	BSPNode* root;
};


#endif