#include <iostream>
#include "Words.h"
#include "Reader.h"
#include "Writer.h"

namespace {
    const int INPUT_ARGV_INDEX = 1;
    const int OUTPUT_ARGV_INDEX = 2;
}

int main(int argc, const char* argv[]) {
    std::list<std::wstring> inputList;
    Reader reader;
    if (!reader.readFile(argv[INPUT_ARGV_INDEX], &inputList)) {
        return 1;
    }
    Words words;
    words.makeMap(&inputList);
    Writer writer;
    if (!writer.writeFile(argv[OUTPUT_ARGV_INDEX], words)) {
        return 1;
    }
    return 0;
}