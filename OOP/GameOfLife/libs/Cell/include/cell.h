#pragma once

namespace cell {
	class Cell
	{
	public:
		Cell();
		Cell(bool isAlive, size_t x, size_t y);
		bool getIsAlive();
		void setIsAlive(bool isAlive);
		size_t getX();
		size_t getY();
	private:
		bool _isAlive;
		size_t _coordinateX;
		size_t _coordinateY;
	};
}