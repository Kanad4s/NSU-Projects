#pragma once
#include <string>
#include <cwctype>

class Line
{
public:
    Line();
    std::wstring popWord();
    std::wstring getLine();
    void setLine(std::wstring newLine);
private:
    std::wstring line;

};

