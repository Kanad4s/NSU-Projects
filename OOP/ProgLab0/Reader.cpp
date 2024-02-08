#include "Reader.h"

bool Reader::readFile(std::string name, std::list<std::wstring>* list)
{
    std::wifstream file;
    file.open(name, std::wifstream::in);
    if (!file) {
        file.close();
        return false;
    }
    file.imbue(std::locale(""));
    std::wstring line;
    while (std::getline(file, line)) {
        list->push_back(line);
    }
    file.close();
    return true;
}
