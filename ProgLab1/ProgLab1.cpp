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
    bitArray2.resize(40, true);
    bitArray.any();
    bitArray.none();
    bitArray.set(10, true);
    bitArray.count();

    std::vector<int> test{ 1,2,3,4,5 };
    test.clear();
    std::cout << "Hello World!\n";
}