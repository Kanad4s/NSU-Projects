#include "Words.h"

Words::Words()
{
    count = 0;
}

void Words::makeMap(std::list<std::string>* list)
{
    for (auto i = list->begin(); i != list->end(); i++) {
        Line line;
        line.line = *i;
        while (!line.line.empty()) {
            std::string word = line.popWord();
            auto isFound = myMap.find(word);
            if (isFound != myMap.end()) {
                myMap.at(word)++;
            }
            else {
                myMap.emplace(word, 1);
                count++;
            }
        }
    }
}
