#pragma once
#include <vector>

class BitArray
{
public:
	BitArray();
	~BitArray();
	BitArray(unsigned long nBits, unsigned long value);
	BitArray(const BitArray& b);

	void swap(BitArray& b);
	void resize(unsigned long nBits, bool value);
	void clear();
	void pushBack(bool bit);
	void set();
	void set(int n, bool value);
	void reset();
	void reset(int n);

	bool any();
	bool none();
	bool empty();

	int count();
	int size();

	void getArray(std::vector<unsigned long> array);

private:
	void insertBit(bool bit, int position);
	void insertBits(bool value, unsigned int begin, unsigned int end);
	std::vector<unsigned long> _array;
	size_t _size;
};

