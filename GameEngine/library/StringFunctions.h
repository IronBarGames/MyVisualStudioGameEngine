#ifndef FCPP_STRFUNC_H
#define FCPP_STRFUNC_H

#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <intrin.h>

typedef std::string string;

inline string boolalpha(const bool& b)
{
	if (b) return "TRUE";
	else return "FALSE";
}

const char* hex(Uint8& i)
{
	char buffer[256];
	sprintf_s(buffer, "%X", i);
	return buffer;
}

const char* GetFullPath()
{
	char filename[MAX_PATH];
	DWORD size = GetModuleFileNameA(NULL, filename, MAX_PATH);
	if (size) return filename;
}

bool& strfind(string& str, const char* find)
{
	bool val(str.find(find) != string::npos);
	return val;
}

/// <summary>
/// checks fi file name exists, add second param of "true" to output the outcome
/// </summary>
bool& CheckFileExists(const char* file, bool output = false)
{
	std::ifstream _file{ file };
	if (output && !_file.good()) std::cout << "file does not exist!\n";
	else if (output && _file.good()) std::cout << "file does exist!\n";
	bool v(_file.good());
	return v;
}

// the size of a file in bytes, does not account for decimals or rounding and will only show base whole number
const int& FileSize(const char* filename)
{
	std::ifstream in(filename, std::ios::ate | std::ios::binary);
	return (in.tellg());
}

void operator+=(std::vector<string>& s1, std::vector<string> s2)
{
	for (int i = 0; i < s2.size(); i++)
	{
		s1.push_back(s2[i]);
	}
}

const string& FPointToStr(SDL_FPoint& point)
{
	string r(std::to_string(point.x) + ", " + std::to_string(point.y));
	return r;
}

#endif