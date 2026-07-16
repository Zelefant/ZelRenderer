#include "Debug.h"
#include <iostream>

void Debug::log(LogType type, std::string text)
{
	std::string prefix;
	switch (type)
	{
	case LogType::RENDER:
		prefix = "[RENDER]";
		break;

	case LogType::ENGINE:
		prefix = "[INFO]";
		break;

	case LogType::BSP:
		prefix = "[BSP]";
		break;

	case LogType::ERROR:
		prefix = "\033[32m[ERROR]";
		break;

	case LogType::WARNING:
		prefix = "\033[33m[WARNING]";
		break;

	case LogType::COMPILE:
		prefix = "[COMPILE]";
		break;

	case LogType::FILE:
		prefix = "[FILE]";
		break;
	}

	std::cout << prefix << " " << text << "\033[0m" << std::endl;
}