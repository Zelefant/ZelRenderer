#include "Wall.h"

Wall::Wall(glm::vec2 vert1, glm::vec2 vert2)
{
	this->vert1 = vert1;
	this->vert2 = vert2;
}

Wall::Wall()
{
	this->vert1 = glm::vec2(1.0f);
	this->vert2 = glm::vec2(0.0f);
}

bool Wall::operator==(const Wall& comp)
{
	if (this->vert1 == comp.vert1)
	{
		if (this->vert2 == comp.vert2)
		{
			return true;
		}
	}
	
	return false;
}
