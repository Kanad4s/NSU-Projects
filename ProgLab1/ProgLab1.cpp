#include <iostream>
#include "BitArray.h"

int main()
{
    BitArray bitArray = BitArray(10, 5);
    BitArray bitArray2 = BitArray(32, 16);
    BitArray ba;
    bitArray2.resize(40, false);
    bitArray.resize(40, true);
    bitArray.set(38, true);
    std::cout << bitArray.toString() << std::endl;
    std::cout << bitArray2.toString() << std::endl;
    ba = bitArray >> 5;
    std::cout << bitArray.toString() << std::endl;
    std::cout << ba.toString() << std::endl;
    
    bitArray ^= bitArray2;
    std::string arrayString = bitArray.toString();
    std::cout << arrayString << std::endl;
    bitArray.clear();

    std::vector<unsigned long> test{2};
    std::vector<unsigned long> test2{ 4 };
    test[0] = test2[0] << 1;
    std::cout << "Hello World!\n";
}