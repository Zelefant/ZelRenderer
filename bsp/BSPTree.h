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

	std::random_device rd;
	std::mt19937 g;

	BSPNode* GenerateBSP(std::vector<Wall> wallList);
	void CreateNewShape(
		std::vector<Wall*>* shape1,
		std::vector<Wall*>* shape2,
		std::vector<Wall*> cross,
		std::vector<Wall*> front,
		std::vector<Wall*> back
	);
	BSPNode* root;
};


#endif