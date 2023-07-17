#ifndef FCPP_MISCFUNC_H
#define FCPP_MISCFUNC_H

//#include <string>
#include <iostream>
#include <Windows.h>

typedef std::string string;
typedef char buf[256];

bool RegisterCommand(int argc, char* args[], string cmd)
{
	if (argc > 0)
	{
		for (int i = 1; i < argc; i++)
		{
			if (_stricmp(args[i], cmd.c_str()) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

#endif