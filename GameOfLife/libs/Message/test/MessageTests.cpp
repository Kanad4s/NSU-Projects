#include "gtest/gtest.h"
#include "../include/message.h"

using namespace messageSpace;

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
