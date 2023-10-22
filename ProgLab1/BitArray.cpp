#include "BitArray.h"
#include <iostream>

namespace {
	int DEFOLT_ARRAY_SIZE = 1;
	int SIZE_LONG = sizeof(long);
	int BITS_IN_BYTE = 8;
	int BITS_IN_LONG = sizeof(long) * 8;
}

BitArray::BitArray() : _array(std::vector<unsigned long>(DEFOLT_ARRAY_SIZE))
{
	_size = BITS_IN_LONG;
}

BitArray::~BitArray()
{
	_size = 0;
	_array.clear();
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
	_array[0] = value;
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

void BitArray::resize(unsigned long nBits, bool value)
{
	if (nBits < 0) {
		throw std::invalid_argument("nBits less than 0");
	}
	unsigned long newSize = nBits / BITS_IN_LONG;
	if (nBits % BITS_IN_LONG != 0) {
		newSize += 1;
	}
	_array.resize(newSize);
	insertBits(value, _size, nBits);
	if (_size > nBits) {
		insertBits(false, nBits, _size);
	}
	_size = nBits;
}

void BitArray::clear()
{
	_array.clear();
	_size = 0;
}

void BitArray::pushBack(bool bit)
{
	if (_size == _array.size() * BITS_IN_LONG) {
		_array.resize(_array.size() + 1);
	}
	insertBit(bit, _size);
	_size += 1;
}

void BitArray::set()
{
	insertBits(true, 0, _size);
}

void BitArray::set(int n, bool value)
{
	if (n >= _size) {
		throw std::invalid_argument("n is out of range");
	}
	else if (n < 0) {
		throw std::invalid_argument("n is negative");
	}
	insertBit(value, n);
}

void BitArray::reset()
{
	insertBits(false, 0, _size);
}

void BitArray::reset(int n)
{
	if (n >= _size) {
		throw std::invalid_argument("n is out of range");
	}
	else if (n < 0) {
		throw std::invalid_argument("n is negative");
	}
	insertBit(false, n);
}

bool BitArray::any() const
{
	bool isAnyBits = false;
	for (int i = 0; i < _array.size(); i++) {
		if (_array[i] != 0) {
			isAnyBits = true;
			break;
		}
	}
	return isAnyBits;
}

bool BitArray::none() const
{
	bool isNoneBits = true;
	for (int i = 0; i < _array.size(); i++) {
		if (_array[i] != 0) {
			isNoneBits = false;
			break;
		}
	}
	return isNoneBits;
}

bool BitArray::empty() const
{
	bool isEmpty;
	_size == 0 ? isEmpty = true : isEmpty = false;
	return isEmpty;
}

int BitArray::count() const
{
	int nTrue = 0;
	for (int i = 0; i < _size; i++) {
		int elementArrayNumber = i / BITS_IN_LONG;
		int elementBitPosition = i % BITS_IN_LONG;
		if (_array[elementArrayNumber] & 1 << elementBitPosition) {
			nTrue++;
		}
	}
	return nTrue;
}

size_t BitArray::size() const
{
	return _size;
}

BitArray& BitArray::operator=(const BitArray& other)
{
	if (this == &other) {
		return *this;
	}
	_array = other._array;
	_size = other._size;
	return *this;
}

BitArray& BitArray::operator~()
{
	for (int i = 0; i < _array.size(); i++) {
		_array[i] = ~_array[i];
	}
	return *this;
}

bool BitArray::operator[](int i) const
{
	if (i >= _size) {
		throw std::invalid_argument("i is out of range");
	}
	else if (i < 0) {
		throw std::invalid_argument("i is negative");
	}
	return _array[i / BITS_IN_LONG] & (1 << (i % BITS_IN_LONG));
}

std::string BitArray::toString() const
{
	std::string arrayString = "";
	for (int i = 0; i < _size; i++) {
		int elementArrayNumber = i / BITS_IN_LONG;
		int elementBitPosition = i % BITS_IN_LONG;
		if (_array[elementArrayNumber] & 1 << elementBitPosition) {
			arrayString += "1";
		}
		else {
			arrayString += "0";
		}
	}
	return arrayString;
}

void BitArray::getArray(std::vector<unsigned long> array)
{
	array = _array;
}

void BitArray::insertBit(bool bit, int position)
{
	int arrayElementNumber = position / BITS_IN_LONG;
	int elementBitPosition = position % BITS_IN_LONG;
	if (bit) {
		_array[arrayElementNumber] |= 1 << elementBitPosition;
	}
	else {
		_array[arrayElementNumber] &= ~(1 << elementBitPosition);
	}

}

void BitArray::insertBits(bool value, size_t begin, size_t end)
{
	for (int i = begin; i < end; i++) {
		insertBit(value, i);
	}
}
