#include "Words.h"

Words::Words()
{
    _count = 0;
}

void Words::makeMap(std::list<std::wstring>* list)
{
    for (auto i = list->begin(); i != list->end(); i++) {
        Line line;
        line.setLine(*i);
        while (!line.getLine().empty()) {
            std::wstring word = line.popWord();
            _count++;
            auto isFound = mapWords.find(word);
            if (isFound != mapWords.end()) {
                mapWords.at(word)++;
            }
            else {
                mapWords.emplace(word, 1);
            }
        }
    }
}

int Words::getCount()
{
    return _count;
}
