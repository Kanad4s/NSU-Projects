#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include "Words.h"

class Writer
{
public:
	bool writeFile(std::string name, Words nWords);
};

