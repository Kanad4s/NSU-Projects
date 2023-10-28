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

BitArray& BitArray::operator&=(const BitArray& b)
{
	if (_size != b._size) {
		throw std::invalid_argument("argument has different size");
	}
	for (int i = 0; i < _array.size(); i++) {
		_array[i] &= b._array[i];
	}
	return *this;
}

BitArray& BitArray::operator|=(const BitArray& b)
{
	if (_size != b._size) {
		throw std::invalid_argument("argument has different size");
	}
	for (int i = 0; i < _array.size(); i++) {
		_array[i] |= b._array[i];
	}
	return *this;
}

BitArray& BitArray::operator^=(const BitArray& b)
{
	if (_size != b._size) {
		throw std::invalid_argument("argument has different size");
	}
	for (int i = 0; i < _array.size(); i++) {
		_array[i] ^= b._array[i];
	}
	return *this;
}

BitArray BitArray::operator~()
{
	BitArray bitArray(*this);
	for (int i = 0; i < _array.size(); i++) {
		bitArray._array[i] = ~_array[i];
	}
	return bitArray;
}

BitArray& BitArray::operator<<(int n)
{
	if (n < 0) {
		throw std::invalid_argument("n is negative");
	}
	n = n % _size;
	if (n == 0) {
		return *this;
	}
	size_t nEmptyElements = n / BITS_IN_LONG;
	BitArray bitArray(*this);
	for (int i = 0; i < nEmptyElements; i++) {
		_array[i] = 0;
	}
	int subtracted = n % BITS_IN_LONG;
	int difference = BITS_IN_LONG - subtracted;
	_array[nEmptyElements] = bitArray._array[0] << subtracted;
	for (size_t i = nEmptyElements + 1; i < _array.size(); i++) {
		_array[i] = bitArray._array[i - nEmptyElements] << subtracted;
		_array[i] += bitArray._array[i - nEmptyElements - 1] >> difference;
	}
	return *this;
}

BitArray& BitArray::operator>>(int n)
{
	if (n < 0) {
		throw std::invalid_argument("n is negative");
	}
	if (n >= _size) {
		insertBits(false, 0, _size);
		return *this;
	}
	else if (n == 0) {
		return *this;
	}
	size_t nEmptyElements = n / BITS_IN_LONG;
	BitArray bitArray(*this);
	for (int i = _array.size() - 1; i > _array.size() - 1 - nEmptyElements; i--) {
		_array[i] = 0;
	}
	int subtracted = n % BITS_IN_LONG;
	int difference = BITS_IN_LONG - subtracted;
	_array[_array.size() - 1 - nEmptyElements] = bitArray._array[_array.size() - 1] >> subtracted;
	for (int i = _array.size() - 2 - nEmptyElements; i >= 0; i--) {
		_array[i] = bitArray._array[i + nEmptyElements] >> subtracted;
		_array[i] += bitArray._array[i + nEmptyElements + 1] << difference;
	}
	return *this;
}

BitArray BitArray::operator<<(int n) const
{
	if (n < 0) {
		throw std::invalid_argument("n is negative");
	}
	n = n % _size;
	if (n == 0) {
		return *this;
	}
	size_t nEmptyElements = n / BITS_IN_LONG;
	BitArray bitArray(*this);
	for (int i = 0; i < nEmptyElements; i++) {
		bitArray._array[i] = 0;
	}
	int subtracted = n % BITS_IN_LONG;
	int difference = BITS_IN_LONG - subtracted;
	bitArray._array[nEmptyElements] = _array[0] << subtracted;
	for (size_t i = nEmptyElements + 1; i < _array.size(); i++) {
		bitArray._array[i] = _array[i - nEmptyElements] << subtracted;
		bitArray._array[i] += _array[i - nEmptyElements - 1] >> difference;
	}
	return bitArray;
	/*BitArray bitArray(*this);
	return bitArray << n;*/
}

BitArray BitArray::operator>>(int n) const
{
	BitArray bitArray(*this);
	return bitArray >> n;
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

bool BitArray::operator==(const BitArray& b)
{
	return (_array == b._array && _size == b._size);
}

bool BitArray::operator!=(const BitArray& b)
{
	return !(*this == b);
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
		if (elementBitPosition == 31) {
			arrayString += " ";
		}
	}
	return arrayString;
}

void BitArray::getArray(std::vector<unsigned long> array)
{
	array = _array;
}

void BitArray::insertBit(bool bit, size_t position)
{
	size_t arrayElementNumber = position / BITS_IN_LONG;
	size_t elementBitPosition = position % BITS_IN_LONG;
	if (bit) {
		_array[arrayElementNumber] |= 1 << elementBitPosition;
	}
	else {
		_array[arrayElementNumber] &= ~(1 << elementBitPosition);
	}

}

void BitArray::insertBits(bool value, size_t begin, size_t end)
{
	for (size_t i = begin; i < end; i++) {
		insertBit(value, i);
	}
}

BitArray operator&(const BitArray& b1, const BitArray& b2)
{
	BitArray bitArray(b1);
	bitArray &= b2;
	return bitArray;
}

BitArray operator|(const BitArray& b1, const BitArray& b2)
{
	BitArray bitArray(b1);
	bitArray |= b2;
	return bitArray;
}

BitArray operator^(const BitArray& b1, const BitArray& b2)
{
	BitArray bitArray(b1);
	bitArray ^= b2;
	return bitArray;
}
