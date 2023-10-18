#include "BitArray.h"
//#include <memory>
#include <iostream>

namespace {
	int DEFOLT_ARRAY_SIZE = sizeof(long int);
	int SIZE_LONG = sizeof(long);
}

BitArray::BitArray() : _array(std::vector<unsigned long>(DEFOLT_ARRAY_SIZE))
{
	/*std::shared_ptr<bool[]> _array1 = std::make_shared<bool[]>(DEFOLT_ARRAY_SIZE);
	BitArray::_array = _array1;*/
	//_array = new int[DEFOLT_ARRAY_SIZE];
	_size = DEFOLT_ARRAY_SIZE;
}

BitArray::~BitArray()
{
	//delete[] _array;
}

BitArray::BitArray(unsigned long nBits, unsigned long value)// : _array(std::vector<int>(nBits / 8 + 1))
{
	if (nBits % 8 == 0) {
		_array = std::vector<unsigned long>(nBits / 8);
	}
	else {
		_array = std::vector<unsigned long>(nBits / 8 + 1);
	}
	//_array = new int[nBits];
	_size = nBits;
	insertBits(value);
}

BitArray::BitArray(const BitArray& b)
{
	_array = b._array;
	_size = b._size;
}

void BitArray::getArray(std::vector<unsigned long> array)
{
	array = _array;
}

void BitArray::nothing()
{
	_array[2] = 10;
}

void BitArray::insertBits(unsigned long value)
{
	_array[0] = value;
	//for (int i = 0; i < sizeof(value) * 8; i++) {
	//	for (int j = 0; j < SIZE_LONG; j++) {

	//	}
	//	if (value & 1 << i) {
	//		//std::cout << i << ", if" << std::endl;
	//		array[i] = 1;
	//	}
	//	else {
	//		//std::cout << i << ", else" << std::endl;
	//		array[i] = 0;
	//	}
	//	//array[i] |= (value | 1 << i);
	//}
}
