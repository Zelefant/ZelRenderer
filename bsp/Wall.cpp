#include "Wall.h"

Wall::Wall(glm::vec3 vert1, glm::vec3 vert2)
{
	this->vert1 = vert1;
	this->vert2 = vert2;
}

Wall::Wall()
{
	this->vert1 = glm::vec3(1.0f);
	this->vert2 = glm::vec3(0.0f);
}