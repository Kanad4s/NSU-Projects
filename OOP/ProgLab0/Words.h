#pragma once
#include <map>
#include <list>
#include "Line.h"

class Words
{
public:
    std::map<std::wstring, int> mapWords;

    Words();
    void makeMap(std::list<std::wstring>* list);
    int getCount();

private:
    int _count;
};

