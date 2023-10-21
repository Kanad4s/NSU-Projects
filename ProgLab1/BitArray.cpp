#include "BitArray.h"
//#include <memory>
#include <iostream>

namespace {
	int DEFOLT_ARRAY_SIZE = sizeof(long);
	int SIZE_LONG = sizeof(long);
	int BITS_IN_BYTE = 8;
	int BITS_IN_LONG = sizeof(long) * 8;
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
}

BitArray::BitArray(unsigned long nBits, unsigned long value)// : _array(std::vector<int>(nBits / 8 + 1))
{
	if (nBits % BITS_IN_LONG == 0) {
		_array = std::vector<unsigned long>(nBits / BITS_IN_LONG);
	}
	else {
		_array = std::vector<unsigned long>(nBits / BITS_IN_LONG + 1);
	}
	_size = nBits;
	insertBits(value);
}

BitArray::BitArray(const BitArray& b)
{
	_array = b._array;
	_size = b._size;
}

void BitArray::swap(BitArray& b)
{
	BitArray tmp = BitArray(b);
	b._array = _array;
	b._size = _size;
	_array = tmp._array;
	_size = tmp._size;
}

void BitArray::resize(int nBits, bool value)
{

	unsigned long difference = nBits - _size;
	_size = nBits;
	int newSize = _size / BITS_IN_LONG;
	_size % BITS_IN_LONG != 0 ? newSize = 1 : newSize = newSize;
	_array.resize(_size / BITS_IN_LONG);
	//_size += difference;
	/*if (difference <= 0) {
		_size += difference;
		_array.erase(_array.begin() + _size / BITS_IN_LONG, _array.end());
	}
	else {
		_size = nBits;
		_array.resize(_size / BITS_IN_LONG);
	}*/
}

void BitArray::getArray(std::vector<unsigned long> array)
{
	array = _array;
}

void BitArray::nothing()
{
	_array[1] = 10;
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
