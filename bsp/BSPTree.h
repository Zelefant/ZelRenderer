#ifndef BSP_GENERATOR_CLASS_H
#define BSP_GENERATOR_CLASS_H

#include <string>
#include <vector>
#include "BSPNode.h"

class BSPTree
{
public:
	BSPTree(std::string map_file_path);

private:

	BSPNode* GenerateBSP(std::vector<Wall> wallList);
	BSPNode* root;
};


#endif