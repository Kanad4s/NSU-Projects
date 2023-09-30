#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <algorithm>
#include "Words.h"

using namespace std;

class Reader 
{
public:
    static bool readFile(string name, list<string>* list) 
    {
        ifstream file;
        file.open(name, ifstream::in);
        if (!file) {
            return false;
        }
        string line = "";
        while (getline(file, line)) {
            list->push_back(line);
        }
        file.close();
        return true;
    }
};

class Writer 
{
public:
    static void writeFile(string name, Words nWords) 
    {
        ofstream file;
        file.open(name, ofstream::out);
        vector<pair<string, int>> words;
        for (auto i = nWords.myMap.begin(); i != nWords.myMap.end(); i++) {
            words.push_back(*i);
        }
        sort(words.begin(), words.end(), compareMapValue);
        for (auto i = words.begin(); i != words.end(); i++) {
            float frequency = static_cast<float>(i->second) / nWords.count * 100;
            file << i->first << ";" << i->second << ";" << frequency << endl;
        }
        file.close();
    }

private:
    static bool compareMapValue(const pair<string, int> &a, const pair<string, int> &b) 
    {
        return a.second > b.second;
    }
};

int main(int argc, const char* argv[]) {

    list<string> mylist;
    if (!Reader::readFile(argv[1], &mylist)) {
        return 1;
    }
    Words nWords;
    nWords.makeMap(&mylist);
    Writer::writeFile(argv[2], nWords);
    return 0;
}
//int main(int argc, char** argv) {
//    try {
//        Game game(argc, argv);
//    }
//    catch (std::invalid_argument const& ex) {
//        std::cout << ex.what() << '\n';
//    }
//    return 0;
//}