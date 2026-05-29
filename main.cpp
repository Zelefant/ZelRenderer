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

	GLfloat normalized_pyramid_vertices[] =
	{
		//     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 1.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,     -0.8f, 0.5f,  0.0f, // Left Side

		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.8f, 0.5f,  0.0f, // Right side

		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 1.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.5f, 1.0f,      0.0f, 0.5f,  0.8f  // Facing side
	};

	GLfloat light_vertices[] =
	{
		-0.1f, -0.1f, -0.1f,
		-0.1f, 0.0f, -0.1f,
		-0.1f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -0.1f, 0.0f,
		0.0f, -0.1f, -0.1f,
		0.0f, 0.0f, -0.1f,
		-0.1f, -0.1f, 0.0f,
	};

	GLuint light_indices[] =
	{
		0, 7, 4,
		0, 5, 4,
		0, 1, 7,
		1, 2, 7,
		1, 6, 2,
		1, 0, 5,
		1, 6, 5,
		2, 3, 6,
		2, 7, 4,
		2, 3, 4,
		6, 5, 4,
		6, 3, 4,
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
		0, 1, 2, // Bottom side
		0, 2, 3, // Bottom side
		4, 6, 5, // Left side
		7, 9, 8, // Non-facing side
		10, 12, 11, // Right side
		13, 15, 14 // Facing side
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

	/*  TEXTURE SHADER  */
	// Create Shader Program
	Shader shaderProgram("default.vert", "default.frag");

	// Create the VAO and bind it
	VAO VAO1;
	VAO1.Bind();

	// Create the VBO and EBO and bind it
	VBO VBO1(normalized_pyramid_vertices, sizeof(normalized_pyramid_vertices));
	EBO EBO1(indices, sizeof(indices));

	// Link the VBO to the VAO and unbind all three
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Texture
	Texture lyonel("lyonel.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	lyonel.texUnit(shaderProgram, "tex0", 0);


	/*  LIGHT SHADER  */
	Shader lightShader("light.vert", "light.frag");
	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(light_vertices, sizeof(light_vertices));
	EBO lightEBO(light_indices, sizeof(light_indices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);


	glm::vec3 lightPos = glm::vec3(0.3f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.id, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	float deltaTime = 0.0f;
	float prevTime = 0.0f;

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

		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");

		lyonel.Bind();

		VAO1.Bind();


		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Light Shader
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(light_indices) / sizeof(int), GL_UNSIGNED_INT, 0);


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