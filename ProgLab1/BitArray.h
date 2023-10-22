#pragma once
#include <vector>
#include <string>

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

	bool any() const;
	bool none() const;
	bool empty() const;

	int count() const;
	size_t size() const;

	BitArray& operator=(const BitArray& b);
	BitArray& operator~();
	bool operator[](int i) const;

	std::string toString() const;

	void getArray(std::vector<unsigned long> array);

private:
	void insertBit(bool bit, int position);
	void insertBits(bool value, size_t begin, size_t end);
	std::vector<unsigned long> _array;
	size_t _size;
};

