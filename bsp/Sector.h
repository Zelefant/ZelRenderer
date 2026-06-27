#ifndef SECTOR_CLASS_H
#define SECTOR_CLASS_H

#include <vector>
#include "Linedef.h"

class Sector
{
public:
	Sector(int id, std::vector<Linedef*> walls, float floorHeight, float ceilingHeight);

private:
	int id;
	float floorHeight;
	float ceilingHeight;

	std::vector<Linedef*> walls;
};



#endif