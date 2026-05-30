#ifndef INPUT_HANDLER_CLASS_H
#define INPUT_HANDLER_CLASS_H

#include <string>
#include <unordered_map>

using std::string;

class InputHandler
{
public:
	InputHandler();

	// Register an action with the key name.
	void RegisterAction(string action, string key);

	// Register an action with the GLFW key enum.
	void RegisterAction(string action, int key);

	// Remove an action from the registry.
	void DeleteAction(string action);

	bool IsActionPressed(string action);

	bool IsActionJustPressed(string action);

	bool IsActionJustReleased(string action);


private:
	std::unordered_map<string, int> inputSet;
};


#endif