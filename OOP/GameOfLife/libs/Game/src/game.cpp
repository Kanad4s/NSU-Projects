#include "../include/game.h"
#include "gtest/gtest.h"

using namespace game;
using namespace gameState;
using namespace commandState;
using namespace field;
using namespace fileParser;
using namespace consoleParser;
using namespace message;

Game::Game()
{
    _field = Field();
    _gameState = GameState();
    _gameMode = onlineMode;
    _iterations = 0;
    _inputFile = standartInput1;
    _outputFile = standartOutut;
}

void Game::initialize(int argc, char* argv[])
{
    Message message;
    message.start();
    setGameMode(argc, argv);
    FileParser fileParser(getInputFile(), _gameState);
    _field.setGameState(_gameState);
}

std::string Game::getInputFile()
{
	return _inputFile;
}

std::string Game::getOuputFile()
{
	return _outputFile;
}

void Game::setGameMode(int argc, char* argv[])
{
    _gameMode = onlineMode;
    _inputFile = standartInput3;
    _outputFile = standartOutut;
	if (argc >= expectedArgc) {
		_gameMode = offlineMode;
		ConsoleParser parser(_iterations, _inputFile, _outputFile, argc, argv);
	}
}

void Game::play()
{
    _field.initWorld();
    _field.printWorld();
    Message message;
    if (_gameMode == onlineMode) {
        CommandState commandState{};
        while (true) {
            commandState.readCommand();
            if (commandState.getCommand() == commands::dump) {
                std::cin >> _outputFile;
                _field.saveWorld(_outputFile);
                message.fileSaved(_outputFile);
            }
            else if (commandState.getCommand() == commands::exit) {
                message.end();
                return;
            }
            else if (commandState.getCommand() == commands::help) {
                message.help();
            }
            else if (commandState.getCommand() == commands::tick) {
                std::string except;
                std::cin >> except;
                try {
                    _iterations = stoi(except);
                }
                catch (std::exception& ex) {
                    std::cout << ex.what() << std::endl;
                    message.unknownCommand();
                    continue;
                }
                while (_iterations > 0) {
                    _field.nextWorld();
                    system("cls");
                    _field.printWorld();
                    _iterations--;
                    Sleep(timeToSleep);
                }
            }
            else if (commandState.getCommand() == commands::tests) {
                ::testing::InitGoogleTest();
                RUN_ALL_TESTS();
            }
            else{
                message.unknownCommand();
            }
        }
    }
    else {
        while (_iterations > 0) {
            _field.nextWorld();
            system("cls");
            _field.printWorld();
            _iterations--; 
            Sleep(timeToSleep);
        }
        _field.saveWorld(_outputFile);
        message.fileSaved(_outputFile);
        return;
    }
}

