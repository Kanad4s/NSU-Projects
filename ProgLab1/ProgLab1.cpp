#include <iostream>
#include "BitArray.h"

int main()
{
    BitArray bitArray = BitArray(32, 7);
    BitArray bitArray2 = BitArray(32, 16);
    BitArray ba;
    bitArray2.resize(40, false);
    bitArray.resize(40, false);
    bitArray2.set(0, true);
    std::cout << bitArray.toString() << std::endl;
    std::cout << bitArray2.toString() << std::endl;
    ba = bitArray | bitArray2;
    std::cout << ba.toString() << std::endl;

    bitArray ^= bitArray2;
    std::string arrayString = bitArray.toString();
    std::cout << arrayString << std::endl;

    std::vector<int> test{ 1,2,3,4,5 };
    test.clear();
    std::cout << "Hello World!\n";
}