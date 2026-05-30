#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include "render/ShaderClass.h"
#include "render/VBO.h"
#include "render/VAO.h"
#include "render/EBO.h"

#include "render/Texture.h"
#include "render/Camera.h"
#include "render/Mesh.h"
#include "render/Model.h"

#include "input/InputHandler.h"



int main(void)
{
	bool fullbright = false;
	unsigned int width = 1920;
	unsigned int height = 1080;

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
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);

	// Initialize Input Handler
	InputHandler input(window);
	
	// Default input actions
	input.RegisterAction("forward", GLFW_KEY_W);
	input.RegisterAction("back", GLFW_KEY_S);
	input.RegisterAction("left", GLFW_KEY_A);
	input.RegisterAction("right", GLFW_KEY_D);
	input.RegisterAction("up", GLFW_KEY_SPACE);
	input.RegisterAction("down", GLFW_KEY_LEFT_CONTROL);
	input.RegisterAction("sprint", GLFW_KEY_LEFT_SHIFT);
	input.RegisterAction("close_window", GLFW_KEY_ESCAPE);

	// Create Default Shader
	Shader defaultShader("shaders/default.vert", "shaders/default.frag");



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.3f, 0.2f, 0.3f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	defaultShader.Activate();
	glUniform4f(glGetUniformLocation(defaultShader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(defaultShader.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1i(glGetUniformLocation(defaultShader.id, "fullbright"), fullbright);

	float deltaTime = 0.0f;
	float prevTime = 0.0f;

	Model model("resources/models/sword/scene.gltf");

	// Loop - Only ends when the window is set to close
	while (!glfwWindowShouldClose(window))
	{
		input.PollInputs();
		if (input.IsActionPressed("close_window"))
		{
			glfwSetWindowShouldClose(window, true);
		}

		// Check for window resizing
		int currWidth, currHeight;
		glfwGetFramebufferSize(window, &currWidth, &currHeight);
		if (currWidth != width || currHeight != height)
		{
			camera.width = currWidth;
			width = currWidth;

			camera.height = currHeight;
			height = currHeight;

			glViewport(0, 0, width, height);
		}

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Simple timer
		double crntTime = glfwGetTime();
		deltaTime = crntTime - prevTime;
		prevTime = crntTime;

		camera.Inputs(window, input, &deltaTime);

		camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		model.Draw(defaultShader, camera);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	defaultShader.Delete();

	// Termination
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}