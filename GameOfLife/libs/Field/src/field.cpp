#include "../include/field.h"

using namespace field;
using namespace cell;
using namespace gameState;

Field::Field(GameState& gameState)
{
	_gameState = &gameState;
}

void Field::initWorld()
{
	_world.reserve(_gameState->getWorldSizeX());
	for (size_t x = 0; x < _gameState->getWorldSizeX(); x++) {
		std::vector<bool> tmp(0);
		for (int y = 0; y < _gameState->getWorldSizeY(); y++) {
			tmp.push_back(false);
		}
		_world.push_back(tmp);
	}
	std::vector<Cell> cells = _gameState->getCells();
	for (int i = 0; i < cells.size(); i++) {
		_world[cells[i].getX()][cells[i].getY()] = true;
	}
}

void Field::saveWorld(std::string outFile)
{
	std::ofstream fout(outFile);
	fout << "#N " << _gameState->getWorldName() << std::endl;
	fout << "#R " << "B" << _gameState->getBirthRule() << "/" << "S" << _gameState->getSurvivalRule() << std::endl;
	fout << "#S " << _gameState->getWorldSizeX() << " " << _gameState->getWorldSizeY() << std::endl;
	for (int y = _gameState->getWorldSizeY() - 1; y >= 0; y--) {
		for (size_t x = 0; x < _gameState->getWorldSizeX(); x++) {
			if (_world[x][y]) {
				fout << "*";
			}
			else {
				fout << " ";
			}
		}
		fout << std::endl;
	}
	fout.close();
}

void Field::printWorld()
{
	for (int y = _gameState->getWorldSizeY() - 1; y >= 0; y--) {
		for (size_t x = 0; x < _gameState->getWorldSizeX(); x++) {
			if (_world[x][y]) {
				std::cout << "*";
			}
			else {
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

void Field::nextWorld()
{
	std::vector<Cell> tmp;
	size_t neighbours = 0;
	size_t leftX, rightX, leftY, rightY;
	for (size_t x = 0; x < _gameState->getWorldSizeX(); x++) {
		for (size_t y = 0; y < _gameState->getWorldSizeY(); y++) {
			neighbours = 0;
			bool isAlive = _world[x][y];
			leftX = (x - 1 + _gameState->getWorldSizeX()) % _gameState->getWorldSizeX();
			rightX = (x + 1 + _gameState->getWorldSizeX()) % _gameState->getWorldSizeX();
			leftY = (y - 1 + _gameState->getWorldSizeY()) % _gameState->getWorldSizeY();
			rightY = (y + 1 + _gameState->getWorldSizeY()) % _gameState->getWorldSizeY();
			neighbours += _world[leftX][leftY] + _world[x][leftY] + _world[rightX][leftY] +
				_world[leftX][y] + _world[rightX][y] + 
				_world[leftX][rightY] + _world[x][rightY] + _world[rightX][rightY];
			if (shouldBeBorn(neighbours) && !isAlive)
				tmp.push_back(Cell(true, x, y));
			if (shouldSurvive(neighbours) && isAlive)
				tmp.push_back(Cell(true, x, y));
			if (!shouldSurvive(neighbours) && isAlive)
				tmp.push_back(Cell(false, x, y));
		}
	}
	for (auto& cell : tmp) {
		if (cell.getIsAlive()) {
			_world[cell.getX()][cell.getY()] = true;
		}
		else {
			_world[cell.getX()][cell.getY()] = false;
		}
	}
	tmp.clear();
}

std::vector<std::vector<bool>> Field::getWorld()
{
	return _world;
}

void field::Field::setGameState(GameState& newGameState)
{
	_gameState = &newGameState;
}

bool Field::shouldBeBorn(int x)
{
	bool shouldBorn = false;
	size_t found = _gameState->getBirthRule().find(std::to_string(x));
	if (found != std::string::npos) {
		shouldBorn = true;
	}
	return shouldBorn;
}

bool Field::shouldSurvive(int x)
{
	bool shouldSurvive = false;
	size_t found = _gameState->getSurvivalRule().find(std::to_string(x));
	if (found != std::string::npos) {
		shouldSurvive = true;
	}
	return shouldSurvive;
}


