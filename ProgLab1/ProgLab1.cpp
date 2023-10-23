#include <iostream>
#include "BitArray.h"

int main()
{
    BitArray bitArray = BitArray(10, 1);
    BitArray bitArray2 = BitArray(32, 16);
    BitArray ba;
    bitArray2.resize(40, false);
    bitArray.resize(40, true);
    bitArray.set(38, true);
    std::cout << bitArray.toString() << std::endl;
    std::cout << bitArray2.toString() << std::endl;
    ba = bitArray << 39;
    std::cout << ba.toString() << std::endl;

    bitArray ^= bitArray2;
    std::string arrayString = bitArray.toString();
    std::cout << arrayString << std::endl;

    std::vector<unsigned long> test{2};
    test[0] = test[0] << 31;
    std::cout << "Hello World!\n";
}