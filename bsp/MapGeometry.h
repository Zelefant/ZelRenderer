#ifndef MAP_GEO_CLASS_H
#define MAP_GEO_CLASS_H

#include <vector>

#include "BSPVertex.h"
#include "Linedef.h"
#include "Sector.h"

class MapGeometry
{
public:

	MapGeometry(
		std::vector<BSPVertex*> vertices,
		std::vector<Linedef*> linedefs,
		std::vector<Sector*> sectors);

	std::vector<BSPVertex*> vertices;
	std::vector<Linedef*> linedefs;
	std::vector<Sector*> sectors;
};


#endif