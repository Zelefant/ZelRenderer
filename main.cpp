#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

int main(void)
{

	// Vertices coordinates
	Vertex vertices[] =
	{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
		Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Vertex lightVertices[] =
	{ //     COORDINATES     //
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
	};

	GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};


	// Initialization
	glfwInit();

	// Sets window hint to OpenGL3.3 with core profile and modern functions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window initialization
	GLFWwindow* window = glfwCreateWindow(width, height, "Zelrenderer Window", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Exception: Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 800);
	glEnable(GL_DEPTH_TEST);

	// Create Shader Program
	Shader shaderProgram("default.vert", "default.frag");



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.3f, 0.2f, 0.3f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	float deltaTime = 0.0f;
	float prevTime = 0.0f;

	Model model("resources/models/sword/scene.gltf");

	// Loop - Only ends when the window is set to close
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Simple timer
		double crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;
		prevTime = crntTime;

		camera.Inputs(window, &deltaTime);

		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		model.Draw(shaderProgram, camera);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	shaderProgram.Delete();

	// Termination
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}