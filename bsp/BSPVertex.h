#ifndef BSP_VERTEX_CLASS_H
#define BSP_VERTEX_CLASS_H

#include <glm/glm.hpp>

class BSPVertex
{
public:

	BSPVertex(int id);
	BSPVertex(int id, float x, float y);
	BSPVertex(int id, glm::vec2 vertex);

	bool operator==(const BSPVertex& comp);

	float getX();
	float getY();
	int GetID();

private:
	int id;
	float x;
	float y;
};

#endif