#include "pch.h"
#include "Cell/include/cell.h"
#include "CommandState/include/commandState.h"
#include "ConsoleParser/include/consoleParser.h"
#include "Field/include/field.h"
#include "FileParser/include/fileParser.h"
#include "Message/include/message.h"

using namespace cell;
using namespace commandState;
using namespace consoleParser;
using namespace field;
using namespace fileParser;
using namespace message;

TEST(cellTests, testConstructor)
{
	Cell cell(false, 0, 0);
	EXPECT_EQ(Cell().getIsAlive(), cell.getIsAlive());
	EXPECT_EQ(Cell().getX(), cell.getX());
	EXPECT_EQ(Cell().getY(), cell.getY());
}

TEST(cellTests, testIsAlive)
{
	Cell cell;
	cell.setIsAlive(true);
	EXPECT_TRUE(cell.getIsAlive());
	cell.setIsAlive(false);
	EXPECT_FALSE(cell.getIsAlive());
}

TEST(cellTests, testCoordinates)
{
	Cell cell(false, 1255, ULONG_MAX);
	EXPECT_EQ(1255, cell.getX());
	EXPECT_EQ(ULONG_MAX, cell.getY());
}

TEST(commandStateTests, testExit)
{
	std::stringstream stream;
	std::cin.rdbuf(stream.rdbuf());
	stream << "exit";
	CommandState commandState;
	commandState.readCommand();
	EXPECT_EQ(commands::exit, commandState.getCommand());
}

TEST(commandStateTests, testTick)
{
	std::stringstream stream;
	std::cin.rdbuf(stream.rdbuf());
	stream << "tick";
	CommandState commandState;
	commandState.readCommand();
	EXPECT_EQ(commands::tick, commandState.getCommand());
}

TEST(commandStateTests, testDump)
{
	std::stringstream stream;
	std::cin.rdbuf(stream.rdbuf());
	stream << "dump";
	CommandState commandState;
	commandState.readCommand();
	EXPECT_EQ(commands::dump, commandState.getCommand());
}

TEST(commandStateTests, testHelp)
{
	std::stringstream stream;
	std::cin.rdbuf(stream.rdbuf());
	stream << "help";
	CommandState commandState;
	commandState.readCommand();
	EXPECT_EQ(commands::help, commandState.getCommand());
}

TEST(consoleParserTests, testConstructor)
{
	char zeroArgv[] = "argvZero";
	char iterationsKey[] = "-i";
	char inputFileKey[] = "-file";
	char outputFileKey[] = "-o";
	char iters[] = "1234567890";
	char fin[] = "inputFILE1230!)($^@./az";
	char fout[] = "oututFILE1-09./($)@!^%!+_";
	int argc = 7;
	char* argv[7] = { zeroArgv, iterationsKey, iters, inputFileKey, fin, outputFileKey, fout };
	int iterations = 0;
	std::string inputFile = "";
	std::string outputFile = "";
	ConsoleParser consoleParser(iterations, inputFile, outputFile, argc, argv);
	EXPECT_EQ(iterations, atoi(iters));
	EXPECT_EQ(inputFile, std::string(fin));
	EXPECT_EQ(outputFile, std::string(fout));
}

TEST(fieldTests, testInitWorld)
{
	GameState gameState;
	std::string inputFile = "Field/fieldTest1.txt";
	FileParser fileParser(inputFile, gameState);
	Field field(gameState);
	field.initWorld();
	std::vector<std::vector<bool>> world = field.getWorld();
	std::vector<Cell> cells = gameState.getCells();
	for (int i = 0; i < cells.size(); i++) {
		EXPECT_EQ(cells[i].getIsAlive(), world[cells[i].getX()][cells[i].getY()]);
	}
}

TEST(filedTests, testPrintWorld)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	std::string inputFile = "Field/fieldTest1.txt";
	GameState gameState;
	FileParser fileParser(inputFile, gameState);
	Field field(gameState);
	field.initWorld();
	field.printWorld();
	EXPECT_EQ(stream.str(), "*    *   *\n"
		"  *  *    \n"
		"* *  *    \n"
		" **       \n"
		"          \n"
		"          \n"
		"          \n"
		"          \n"
		"        * \n"
		"*        *\n");
}

TEST(fieldTests, testNextWorld)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	std::string inputFile = "Field/fieldTest2.txt";
	GameState gameState;
	FileParser fileParser(inputFile, gameState);
	Field field(gameState);
	field.initWorld();
	field.nextWorld();
	field.printWorld();
	EXPECT_EQ(stream.str(), "     \n"
		" *   \n"
		"  ** \n"
		" **  \n"
		"     \n"
		"     \n"
		"     \n");
}

TEST(fileParserTests, testConstructor)
{
	GameState gameState;
	std::string inputFile;
	inputFile = "FileParser/fileTest1.txt";
	FileParser fileParser(inputFile, gameState);
	std::vector<Cell> cells = { Cell(true, 0, 7), Cell(true, 1, 6), Cell(true, 2, 6), Cell(true, 2, 7), Cell(true, 2, 8) };
	EXPECT_EQ(gameState.getWorldName(), "MyUniverse number 1");
	EXPECT_EQ(gameState.getSurvivalRule(), "S12345678");
	EXPECT_EQ(gameState.getBirthRule(), "B3");
	EXPECT_EQ(gameState.getWorldSizeX(), 3);
	EXPECT_EQ(gameState.getWorldSizeY(), 10000);
	auto cellsParser = gameState.getCells();
	for (int i = 0; i < cells.size(); i++) {
		EXPECT_EQ(cellsParser[i].getIsAlive(), cells[i].getIsAlive());
		EXPECT_EQ(cellsParser[i].getX(), cells[i].getX());
		EXPECT_EQ(cellsParser[i].getY(), cells[i].getY());
	}
}

TEST(messageTests, testStart)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.start();
	EXPECT_EQ(stream.str(), "Hello, you are in the game of life!\n"
		"There ara 2 mods: online and offline\n"
		"online mode: just launch the game and use commands!\n"
		"offline mode: start the game with -file <input file> -i <count of iterations> -o <output file>\n"
		"Use following commands:\n"
		"dump <filename> - save your world in the file\n"
		"tick <n> - make n iterations and show result\n"
		"exit - finish sesion\n"
		"help - show game commands\n");
}

TEST(messageTests, testHelp)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.help();
	EXPECT_EQ(stream.str(), "Use following commands:\n"
		"dump <filename> - save your world in the file\n"
		"tick <n> - make n iterations and show result\n"
		"exit - finish sesion\n"
		"help - show game commands\n");
}

TEST(messageTests, testErrorFormatFile)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.errorFormatFile();
	EXPECT_EQ(stream.str(), "ERROR:\n"
		"File must be in #Life 1.06 format\n");
}

TEST(messageTests, testErrorNoRules)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.errorNoRules();
	EXPECT_EQ(stream.str(), "ERROR:\n"
		"Rules shoud be written: #R Bx/Sy, where x, y - sequences of different numbers from the set {0...8}.\n");
}

TEST(messageTests, testErrorNoCells)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.errorNoCells();
	EXPECT_EQ(stream.str(), "ERROR:\n"
		"There are no coordinates of alive cells\n"
		"Each coordinate line should be of the form:\n"
		"x y\n"
		"where x and y are the x and y coordinates of an alive cell\n");
}

TEST(messageTests, testErrorNoSize)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.errorNoSize();
	EXPECT_EQ(stream.str(), "ERROR:\n"
		"There is no size of the world\n"
		"Write \"#S x y\", where x and y - width and height of the world\n");
}

TEST(messageTests, testUnknownCommand)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.unknownCommand();
	EXPECT_EQ(stream.str(), "There is no such command\n");
}

TEST(messageTests, testFileSaved)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	std::string fileName = "testFile name 124/.?#@!$%";
	message.fileSaved(fileName);
	EXPECT_EQ(stream.str(), "file saved in: " + fileName + "!\n");
}

TEST(messageTests, testEnd)
{
	std::stringstream stream;
	std::cout.rdbuf(stream.rdbuf());
	Message message;
	message.end();
	EXPECT_EQ(stream.str(), "Thank you for playing, can't wait to see you again :)\n");
}


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}