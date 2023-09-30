#pragma once
#include <map>
#include <list>
#include "Line.h"

class Words
{
public:
    int count;
    std::map<std::string, int> myMap;

    Words();
    void makeMap(std::list<std::string>* list);
};

