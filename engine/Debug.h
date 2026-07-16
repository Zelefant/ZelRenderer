#ifndef DEBUG_HEADER_H
#define DEBUG_HEADER_H

#include <string>

enum class LogType
{
	RENDER,
	WARNING,
	ERROR,
	ENGINE,
	BSP,
	COMPILE,
	FILE,
};

class Debug
{
public:

	static void log(LogType type, std::string text);
};


#endif