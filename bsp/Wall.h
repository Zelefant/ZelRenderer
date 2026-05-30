#ifndef WALL_CLASS_H
#define WALL_CLASS_H

#include <glm/glm.hpp>

class Wall
{
public:
	Wall();
	Wall(glm::vec3 vert1, glm::vec3 vert2);

private:
	glm::vec3 vert1, vert2;
};




#endif