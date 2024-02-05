#pragma once
#include "../../GameState/include/gameState.h"

using namespace gameState;

namespace fileParser {
	const int readLineSize = 256;

	class FileParser
	{
	public:
		FileParser(std::string inputFile, GameState& gameState);
	};
}

