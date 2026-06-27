#ifndef SECTOR_CLASS_H
#define SECTOR_CLASS_H

#include <vector>
#include "Linedef.h"

class Sector
{
public:


private:
	float floorHeight;
	float ceilingHeight;

	std::vector<Linedef*> walls;
};



#endif