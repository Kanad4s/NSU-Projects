#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include "../../Cell/include/cell.h"
#include "../../Message/include/message.h"

using namespace cell;

namespace gameState {
	namespace gameStateRegex {
		const std::regex fileFormat("^#Life 1.06$");
		const std::regex worldName("^#N (.+)$");
		const std::regex rules("^#R (B[0-8]+)/(S[0-8]+)$");
		const std::regex coordinates("([0-9]+) ([0-9]+)$");
		const std::regex worldSize("^#S ([0-9]+) ([0-9]+)$");
	}

	class GameState
	{
	public:
		GameState();
		std::string getBirthRule();
		std::string getSurvivalRule();
		std::string getWorldName();
		size_t getWorldSizeX();
		size_t getWorldSizeY();
		std::vector<Cell> getCells();
		bool setRules(char readLine[], std::regex rules);
		bool setWorldName(char readLine[], std::regex worldName);
		bool setSize(char readLine[], std::regex size);
		bool setCells(char readLine[], std::regex coordinates);
	private:
		std::string _birthRule;
		std::string _survivalRule;
		std::string _worldName;
		size_t _wolrdSizeX;
		size_t _worldSizeY;
		std::vector<Cell> _Cells;
	};
}
