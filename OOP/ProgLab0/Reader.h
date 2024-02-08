#pragma once
#include <string>
#include <list>
#include <fstream>

class Reader
{
public:
	bool readFile(std::string name, std::list<std::wstring>* list);
};

