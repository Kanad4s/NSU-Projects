#include "../include/cell.h"

using namespace cell;

Cell::Cell()
{
	_isAlive = false;
	_coordinateX = 0;
	_coordinateY = 0;
}

Cell::Cell(bool isAlive, size_t x, size_t y)
{
	_isAlive = isAlive;
	_coordinateX = x;
	_coordinateY = y;
}

bool Cell::getIsAlive()
{
	return _isAlive;
}

void Cell::setIsAlive(bool isAlive)
{
	_isAlive = isAlive;
}

size_t Cell::getX()
{
	return _coordinateX;
}

size_t Cell::getY()
{
	return _coordinateY;
}
