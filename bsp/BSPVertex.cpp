#include "BSPVertex.h"

// Constructors
BSPVertex::BSPVertex()
{
	this->x = 0.0;
	this->y = 0.0;
}

BSPVertex::BSPVertex(float x, float y)
{
	this->x = x;
	this->y = y;
}

BSPVertex::BSPVertex(glm::vec2 vertex)
{
	this->x = vertex.x;
	this->y = vertex.y;
}


// Properties
float BSPVertex::getX()
{
	return this->x;
}

float BSPVertex::getY()
{
	return this->y;
}


// Operators
bool BSPVertex::operator==(const BSPVertex& comp)
{
	if (this->x == comp.x)
	{
		if (this->y == comp.y)
		{
			return true;
		}
	}

	return false;
}