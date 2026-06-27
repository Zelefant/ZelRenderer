#include "BSPNode.h"


BSPNode::BSPNode(Linedef* wall)
{
	this->wall = wall;
} 

Linedef* BSPNode::GetLinedef()
{
	return this->wall;
}