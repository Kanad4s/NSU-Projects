#include "Writer.h"

bool Writer::writeFile(std::string name, Words words)
{
    std::wofstream file;
    file.open(name, std::wofstream::out);
    if (!file) {
        file.close();
        return false;
    }
    file.imbue(std::locale(""));
    std::vector<std::pair<std::wstring, int>> pairs;
    for (auto i = words.mapWords.begin(); i != words.mapWords.end(); i++) {
        pairs.push_back(*i);
    }
    sort(pairs.begin(), pairs.end(), [](const std::pair<std::wstring, int>& a, const std::pair<std::wstring, int>& b) {
        return a.second > b.second;
    });
    for (auto i = pairs.begin(); i != pairs.end(); i++) {
        float frequency = static_cast<float>(i->second) / words.getCount() * 100;
        file << i->first << L";" << i->second << L";" << frequency << std::endl;
    }
    file.close();
    return true;
}

