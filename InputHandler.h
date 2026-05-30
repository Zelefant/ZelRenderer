#ifndef INPUT_HANDLER_CLASS_H
#define INPUT_HANDLER_CLASS_H

#include <GLFW/glfw3.h>

#include <string>
#include <unordered_map>

using std::string;

typedef enum InputPoll
{
	PRESSED,
	RELEASED,
	JUST_PRESSED,
	JUST_RELEASED, // unused
};

class InputHandler
{
public:
	InputHandler(GLFWwindow* window);

	// Register an action with the GLFW key enum.
	bool RegisterAction(string action, int key);

	// Remove an action from the registry.
	void DeleteAction(string action);

	bool IsActionPressed(string action);

	bool IsActionJustPressed(string action);

	bool IsActionReleased(string action);

	// Should run every frame in the main loop
	void PollInputs();


private:
	std::unordered_map<string, int> inputSet;
	std::unordered_map<string, InputPoll> poll;
	GLFWwindow* window;
};


#endif