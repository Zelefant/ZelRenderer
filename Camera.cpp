#include "Camera.h"


Camera::Camera(int width, int height, glm::vec3 position)
{
	this->width = width;
	this->height = height;
	Position = position;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Creates the view and projection matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	// Provides them with the intended orientation and position as well as perspective details
	view = glm::lookAt(Position, Position + Orientation, Up);
	proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	// Assign to cam matrix property
	this->CamMatrix = proj * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.id, uniform), 1, GL_FALSE, glm::value_ptr(CamMatrix));
}

void Camera::Inputs(GLFWwindow* window, InputHandler input, float* deltaTime)
{
	float dt;
	if (deltaTime == NULL)
		dt = 1.0f;
	else
		dt = *deltaTime;

	// Handles key inputs
	if (input.IsActionPressed("forward"))
	{
		Position += speed * Orientation * dt;
	}
	if (input.IsActionPressed("left"))
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up)) * dt;
	}
	if (input.IsActionPressed("back"))
	{
		Position += speed * -Orientation * dt;
	}
	if (input.IsActionPressed("right"))
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up)) * dt;
	}
	if (input.IsActionPressed("up"))
	{
		Position += speed * Up * dt;
	}
	if (input.IsActionPressed("down"))
	{
		Position += speed * -Up * dt;
	}
	if (input.IsActionPressed("sprint"))
	{
		speed = defaultShiftSpeed;
	}
	else if (input.IsActionReleased("sprint"))
	{
		speed = defaultSpeed;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
	
}