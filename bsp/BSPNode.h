#ifndef BSP_NODE_CLASS_H
#define BSP_NODE_CLASS_H

#include <vector>
#include "Wall.h"

class BSPNode
{
public:
	BSPNode(Wall wall);

private:
	Wall wall;
	BSPNode* leftNode = nullptr;
	BSPNode* rightNode = nullptr;
};


#endif