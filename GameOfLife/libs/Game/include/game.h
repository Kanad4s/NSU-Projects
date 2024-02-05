#ifndef game_h
#define game_h

#include "../../CommandState/include/commandState.h"
#include "../../ConsoleParser/include/consoleParser.h"
#include "../../FileParser/include/fileParser.h"
#include "../../Field/include/field.h"
#include <Windows.h>

namespace game {
	const int onlineMode = 0;
	const int offlineMode = 1;
	const int timeToSleep = 200;
	const int expectedArgc = 7;
	const std::string standartInput1 = "./config/file.txt";
	const std::string standartInput2 = "./config/file2.txt";
	const std::string standartInput3 = "./config/file3.txt";
	const std::string standartOutut = "./config/exit.txt";

	class Game {
	public:
		Game();
		void initialize(int argc, char* argv[]);
		void play();
		void setGameMode(int argc, char* argv[]);
		std::string getInputFile();
		std::string getOuputFile();
	private:
		int _gameMode;
		int _iterations;
		std::string _inputFile;
		std::string _outputFile;
		field::Field _field;
		gameState::GameState _gameState;
	};
}
#endif





