#ifndef WALL_CLASS_H
#define WALL_CLASS_H

#include <glm/glm.hpp>

class Wall
{
public:
	Wall();
	Wall(glm::vec2 vert1, glm::vec2 vert2, bool visible = true);

	bool operator==(const Wall& comp);

	glm::vec2 vert1, vert2;

	bool visible;
};




#endif