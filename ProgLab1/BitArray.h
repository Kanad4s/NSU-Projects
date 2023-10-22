#pragma once
#include <vector>
#include <string>
#include <stdexcept>

class BitArray
{
public:
	BitArray();
	~BitArray();
	explicit BitArray(unsigned long nBits, unsigned long value);
	BitArray(const BitArray& b);

	void swap(BitArray& b);
	void resize(unsigned long nBits, bool value);
	void clear();
	void pushBack(bool bit);
	void set();
	void set(int n, bool value);
	void reset();
	void reset(int n);
	void getArray(std::vector<unsigned long> array);

	bool any() const;
	bool none() const;
	bool empty() const;
	bool operator[](int i) const;

	bool operator==(const BitArray& b);
	bool operator!=(const BitArray& b);

	int count() const;
	size_t size() const;

	std::string toString() const;

	BitArray& operator=(const BitArray& b);
	BitArray& operator&=(const BitArray& b);
	BitArray& operator|=(const BitArray& b);
	BitArray& operator^=(const BitArray& b);
	BitArray& operator<<(int n);

	BitArray operator~();
	
private:
	void insertBit(bool bit, int position);
	void insertBits(bool value, size_t begin, size_t end);
	std::vector<unsigned long> _array;
	size_t _size;
};

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);
