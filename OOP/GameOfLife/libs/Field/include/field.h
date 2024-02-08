#pragma once
#include "../../GameState/include/gameState.h"

using namespace gameState;

namespace field {
	class Field
	{
	public:
		Field() = default;
		Field(GameState& gameState);
		void initWorld();
		void saveWorld(std::string outFile);
		void printWorld();
		void nextWorld();
		std::vector<std::vector<bool>> getWorld();
		void setGameState(GameState& newGameState);
	private:
		bool shouldBeBorn(int x);
		bool shouldSurvive(int x);
		GameState* _gameState;
		std::vector<std::vector<bool>> _world;
	};
}

