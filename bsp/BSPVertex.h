#ifndef BSP_VERTEX_CLASS_H
#define BSP_VERTEX_CLASS_H

#include <glm/glm.hpp>

class BSPVertex
{
public:

	BSPVertex();
	BSPVertex(float x, float y);
	BSPVertex(glm::vec2 vertex);

	bool operator==(const BSPVertex& comp);

	float getX();
	float getY();

private:
	float x;
	float y;
};

#endif