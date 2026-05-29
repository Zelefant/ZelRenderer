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

const unsigned int width = 800;
const unsigned int height = 800;

int main(void)
{

	GLfloat vertices[] =
	{
		//			     Coordinates			   /		Colors			//	Texture Coordinates
		-0.5f, 0.0f, -0.5f,								1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.0f, 0.5f,								1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,								1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		0.5f, 0.0f, -0.5f,								1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		0.0f, 0.8f, 0.0f,								1.0f, 0.0f, 0.0f,		0.5f, 1.0f,

	};

	GLfloat square_vertices[] =
	{
		//			     Coordinates			   /		Colors			//	Texture Coordinates
		-0.5f, -0.5f, 0.0f,								1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,								0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,								0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,								1.0f, 1.0f, 0.0f,		1.0f, 0.0f
	};

	GLfloat triforce_vertices[] =
	{
		//			     Coordinates			   /		Colors			//
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.4f, 0.0f, 0.3f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,			0.1f, 0.6f, 0.3f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,		1.0f, 0.2f, 0.3f,

		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.5f, 0.8f, 0.3f,
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.1f, 0.3f, 0.3f,
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,			0.0f, 0.8f, 0.3f
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	GLuint square_indices[] =
	{
		0, 2, 1,
		0, 3, 2
	};

	GLuint triforce_indices[] =
	{
		0, 3, 5,
		3, 2, 4,
		5, 4, 1
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

	// Create the VAO and bind it
	VAO VAO1;
	VAO1.Bind();

	// Create the VBO and EBO and bind it
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Link the VBO to the VAO and unbind all three
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Texture
	Texture lyonel("lyonel.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	lyonel.texUnit(shaderProgram, "tex0", 0);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	float deltaTime = 0.0f;
	float prevTime = 0.0f;

	// Loop - Only ends when the window is set to close
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		// Simple timer
		double crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;
		prevTime = crntTime;

		camera.Inputs(window, &deltaTime);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		lyonel.Bind();

		VAO1.Bind();


		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	lyonel.Delete();
	shaderProgram.Delete();

	// Termination
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}