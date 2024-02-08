#include "Line.h"
#include <cwctype>
#include <iostream>

Line::Line() {
    line = L"";
}

std::wstring Line::popWord()
{
    std::wstring word = L"";
    int i = 0;
    while (i < line.size() && iswalnum(line[i])) {
        i++;
    }
    word = line.substr(0, i);
    while (i < line.size() && !(iswalnum(line[i]))) {
        ++i;
    }
    line.erase(0, i);
    return word;
}

std::wstring Line::getLine()
{
    return line;
}

void Line::setLine(std::wstring newLine)
{
    line = newLine;
}







