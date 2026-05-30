#include "InputHandler.h"

InputHandler::InputHandler(GLFWwindow* window)
{
	this->inputSet = std::unordered_map<string, int>();
	this->window = window;
	std::unordered_map<string, InputPoll> poll = std::unordered_map<string, InputPoll>();
}

bool InputHandler::RegisterAction(string action, int key)
{
	// if action does not exist in input set already
	if (this->inputSet.find(action) == this->inputSet.end())
	{
		this->inputSet[action] = key;
		this->poll[action] = InputPoll::RELEASED;
		return true;
	}

	return false;
}

void InputHandler::DeleteAction(string action)
{
	if (this->inputSet.find(action) != this->inputSet.end())
	{
		this->inputSet.erase(action);
	}
}

void InputHandler::PollInputs()
{
	for (const auto& pair : inputSet)
	{
		if (glfwGetKey(window, pair.second) == GLFW_PRESS && (poll[pair.first] == InputPoll::RELEASED))
		{
			// Button was just pressed.
			poll[pair.first] = InputPoll::JUST_PRESSED;
		}
		else if (glfwGetKey(window, pair.second) == GLFW_PRESS && (poll[pair.first] == InputPoll::JUST_PRESSED))
		{
			// Button has been pressed for a frame.
			poll[pair.first] = InputPoll::PRESSED;
		}
		else
		{
			// Button was released.
			poll[pair.first] = InputPoll::RELEASED;
		}
	}
}

bool InputHandler::IsActionPressed(string action)
{
	InputPoll status = poll[action];
	if (status == InputPoll::PRESSED || status == InputPoll::JUST_PRESSED)
	{
		return true;
	}

	return false;
}

bool InputHandler::IsActionJustPressed(string action)
{
	InputPoll status = poll[action];
	if (status == InputPoll::JUST_PRESSED)
	{
		return true;
	}
	
	return false;
}

bool InputHandler::IsActionReleased(string action)
{
	InputPoll status = poll[action];
	if (status == InputPoll::RELEASED)
	{
		return true;
	}
	
	return false;
}