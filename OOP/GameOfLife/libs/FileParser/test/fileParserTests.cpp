#include "../include/fileParser.h"
#include "gtest/gtest.h"

using namespace gameStateSpace;
using namespace cellSpace;
using namespace fileParser;

TEST(fileParserTests, testConstructor)
{
	GameState gameState;
	std::string inputFile;
	inputFile = "./libs/FileParser/test/fileTest1.txt";
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