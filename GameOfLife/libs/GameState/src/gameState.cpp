#include "../include/gameState.h"

using namespace gameState;
using namespace cell;

GameState::GameState()
{
	_birthRule = "";
	_survivalRule = "";
	_worldName = "";
	_wolrdSizeX = 0;
	_worldSizeY = 0;
}

std::string GameState::getBirthRule()
{
	return _birthRule;
}

std::string GameState::getSurvivalRule()
{
	return _survivalRule;
}

std::string GameState::getWorldName()
{
	return _worldName;
}

size_t GameState::getWorldSizeX()
{
	return _wolrdSizeX;
}

size_t GameState::getWorldSizeY()
{
	return _worldSizeY;
}

std::vector<Cell> GameState::getCells()
{
	return _Cells;
}

bool GameState::setRules(char readLine[], std::regex rules)
{
	bool isSet = true;
	std::string line(readLine);
	auto iter = std::sregex_iterator(line.begin(), line.end(), rules);
	if (iter->empty()) {
		isSet = false;
	}
	else {
		_birthRule = iter->_At(1).str();
		_survivalRule = iter->_At(2).str();
	}
	return isSet;
}

bool GameState::setWorldName(char readLine[], std::regex universeName)
{
	bool isSet = true;
	std::string line(readLine);
	auto iter = std::sregex_iterator(line.begin(), line.end(), universeName);
	if (iter->empty()) {
		isSet = false;
		_worldName = "Game of life";
	}
	else {
		_worldName = iter->_At(1).str();
	}
	return isSet;
}

bool GameState::setSize(char readLine[], std::regex size)
{
	bool isSet = true;
	std::string line(readLine);
	auto iter = std::sregex_iterator(line.begin(), line.end(), size);
	if (iter->empty()) {
		isSet = false;
	}
	else {
		_wolrdSizeX = stoi(iter->_At(1).str());
		_worldSizeY = stoi(iter->_At(2).str());
	}
	return isSet;
}

bool GameState::setCells(char readLine[], std::regex coordinates)
{
	bool isSet = false;
	std::string line(readLine);
	std::sregex_iterator iter = std::sregex_iterator(line.begin(), line.end(), coordinates);
	if (!iter->empty()) {
		int x = stoi(iter->_At(1).str()) % _wolrdSizeX;
		int y = stoi(iter->_At(2).str()) % _worldSizeY;
		_Cells.push_back(Cell(true, x, y));
		isSet = true;
	}
	return isSet;
}

