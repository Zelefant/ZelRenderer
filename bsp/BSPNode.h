#ifndef BSP_NODE_CLASS_H
#define BSP_NODE_CLASS_H

#include <vector>
#include "Linedef.h"

class BSPNode
{
public:
	BSPNode(Linedef wall);

private:
	Linedef wall;
	BSPNode* leftNode = nullptr;
	BSPNode* rightNode = nullptr;
};


#endif