#ifndef BSP_RENDERER_CLASS_H
#define BSP_RENDERER_CLASS_H

#include "BSPTree.h"
#include <GLFW/glfw3.h>

class BSPRenderer
{
public:
	BSPRenderer(GLFWwindow* window, BSPTree tree);

	void RenderFromLocation(glm::vec2 playerPosition);


private:

	GLFWwindow* window;
	BSPTree tree;

};


#endif