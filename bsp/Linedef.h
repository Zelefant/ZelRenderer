#ifndef WALL_CLASS_H
#define WALL_CLASS_H

#include <glm/glm.hpp>
#include "BSPVertex.h"

class Linedef
{
public:
	Linedef(bool visible = true);
	Linedef(glm::vec2 vert1, glm::vec2 vert2, bool visible = true);
	Linedef(BSPVertex* vert1, BSPVertex* vert2, bool visible);

	~Linedef();

	bool operator==(const Linedef& comp);

	BSPVertex* Start();
	BSPVertex* End();

	bool visible;

private:
	BSPVertex* start;
	BSPVertex* end;



};




#endif