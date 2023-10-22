#include <iostream>
#include "BitArray.h"

int main()
{
    BitArray bitArray = BitArray(32, 7);
    int array[10];
    //bitArray.getArray(array);
    std::cout << sizeof(unsigned long int) << " " << LONG_MAX << std::endl;
    BitArray bitArray2 = BitArray(bitArray);
    bitArray.swap(bitArray2);
    int b = -3;
    BitArray ba;
    bitArray2.resize(40, true);
    bitArray.set(10, 7);
    bitArray.any();
    bitArray.none();
    bitArray.set(10, true);
    bitArray.count();
    std::string arrayString = bitArray2.toString();
    std::cout << arrayString << std::endl;
    try {
        bool value = bitArray2[40];

    }
    catch (const std::exception& ex) {
        std::cout << "???\n";
    }
    ~bitArray2;
    ba = bitArray2;
    ba.reset(30);
    arrayString = ba.toString();
    std::cout << arrayString << std::endl;

    std::vector<int> test{ 1,2,3,4,5 };
    test.clear();
    std::cout << "Hello World!\n";
}