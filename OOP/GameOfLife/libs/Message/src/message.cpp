#include "../include/message.h"

using namespace message;

void Message::start() const
{
	std::cout << "Hello, you are in the game of life!" << std::endl;
	std::cout << "There ara 2 mods: online and offline" << std::endl;
	std::cout << "online mode: just launch the game and use commands!" << std::endl;
	std::cout << "offline mode: start the game with -file <input file> -i <count of iterations> -o <output file>" << std::endl;
	help();
}

void Message::help() const
{
	std::cout << "Use following commands:" << std::endl;
	std::cout << "dump <filename> - save your world in the file" << std::endl;
	std::cout << "tick <n> - make n iterations and show result" << std::endl;
	std::cout << "exit - finish sesion" << std::endl;
	std::cout << "help - show game commands" << std::endl;
}

void Message::errorFormatFile() const
{
	std::cout << "ERROR:" << std::endl;
	std::cout << "File must be in #Life 1.06 format" << std::endl;
}

void Message::errorNoRules() const
{
	std::cout << "ERROR:" << std::endl;
	std::cout << "Rules shoud be written: #R Bx/Sy, where x, y - sequences of different numbers from the set {0...8}." << std::endl;
}

void Message::errorNoCells() const
{
	std::cout << "ERROR:" << std::endl;
	std::cout << "There are no coordinates of alive cells" << std::endl;
	std::cout << "Each coordinate line should be of the form:" << std::endl;
	std::cout << "x y" << std::endl;
	std::cout << "where x and y are the x and y coordinates of an alive cell" << std::endl;
}

void Message::errorNoSize() const
{
	std::cout << "ERROR:" << std::endl;
	std::cout << "There is no size of the world" << std::endl;
	std::cout << "Write \"#S x y\", where x and y - width and height of the world" << std::endl;
}

void Message::unknownCommand() const
{
	std::cout << "There is no such command" << std::endl;
}

void Message::fileSaved(std::string outputFile) const
{
	std::cout << "file saved in: " << outputFile << "!" << std::endl;
}

void Message::end() const
{
	std::cout << "Thank you for playing, can't wait to see you again :)" << std::endl;
}

