#include "Sector.h"


Sector::Sector(int id, std::vector<Linedef*> walls, float floorHeight, float ceilingHeight)
{
	this->id = id;
	this->walls = walls;
	this->floorHeight = floorHeight;
	this->ceilingHeight = ceilingHeight;
}