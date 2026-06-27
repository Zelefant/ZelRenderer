#include "BSPVertex.h"

// Constructors
BSPVertex::BSPVertex(int id)
{
	this->id = id;
	this->x = 0.0;
	this->y = 0.0;
}

BSPVertex::BSPVertex(int id, float x, float y)
{
	this->id = id;
	this->x = x;
	this->y = y;
}

BSPVertex::BSPVertex(int id, glm::vec2 vertex)
{
	this->id = id;
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

int BSPVertex::GetID()
{
	return this->id;
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