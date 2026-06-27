#ifndef WALL_CLASS_H
#define WALL_CLASS_H

#include <glm/glm.hpp>
#include "BSPVertex.h"

class Linedef
{
public:
	Linedef(int id, BSPVertex* vert1, BSPVertex* vert2, bool visible = true);

	bool operator==(const Linedef& comp);

	BSPVertex* Start();
	BSPVertex* End();
	int GetID();

	bool visible;

private:
	int id;
	BSPVertex* start;
	BSPVertex* end;



};




#endif