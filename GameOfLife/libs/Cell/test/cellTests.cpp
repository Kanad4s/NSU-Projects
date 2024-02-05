#include "../include/cell.h"
#include "gtest/gtest.h"

using namespace cellSpace;

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
