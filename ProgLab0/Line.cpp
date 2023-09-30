#include "Line.h"

Line::Line() {
    line = "";
}

std::string Line::popWord()
{
    std::string word = "";
    int i = 0;
    while (i < line.size() && isalpha(line[i])) {
        i++;
    }
    word = line.substr(0, i);
    while (i < line.size() && !isalpha(line[i])) {
        i++;
    }
    line.erase(0, i);
    return word;
}
