#include "../include/commandState.h"
#include "gtest/gtest.h"

using namespace commandState;

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
