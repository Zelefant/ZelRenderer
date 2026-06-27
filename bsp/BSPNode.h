#ifndef BSP_NODE_CLASS_H
#define BSP_NODE_CLASS_H

#include <vector>
#include "Linedef.h"

class BSPNode
{
public:
	BSPNode(Linedef* wall);

	BSPNode* leftNode = nullptr;
	BSPNode* rightNode = nullptr;

	Linedef* GetLinedef();

private:
	Linedef* wall;
};


#endif