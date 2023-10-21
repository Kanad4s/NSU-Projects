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
	void resize(int nBits, bool value);

	void getArray(std::vector<unsigned long> array);
	void nothing();

private:
	//std::shared_ptr<bool[]> _array;
	void insertBits(unsigned long value);
	std::vector<unsigned long> _array;
	//int _array[];
	size_t _size;
};

