#include "../../FileParser/include/fileParser.h"
#include "../include/field.h"
#include "gtest/gtest.h"

using namespace gameStateSpace;
using namespace fileParser;
using namespace fieldSpace;
using namespace cellSpace;

TEST(fieldTests, testInitWorld)
{
	GameState gameState;
	std::string inputFile = "./libs/Field/test/fieldTest1.txt";
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
	std::string inputFile = "./libs/Field/test/fieldTest1.txt";
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
	std::string inputFile = "./libs/Field/test/fieldTest2.txt";
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