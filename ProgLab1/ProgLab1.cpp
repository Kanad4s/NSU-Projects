#include <iostream>
#include "BitArray.h"

int main()
{
    BitArray bitArray = BitArray(40, 13);
    int array[10];
    //bitArray.getArray(array);
    int a = 0;
    std::cout << sizeof(unsigned long int) << " " << sizeof(long) << std::endl;
    BitArray bitArray2 = BitArray(bitArray);
    bitArray.nothing();
    bitArray.swap(bitArray2);

    std::vector<int> test{ 1,2,3,4,5 };
    test.resize(3);
    std::cout << "Hello World!\n";
}