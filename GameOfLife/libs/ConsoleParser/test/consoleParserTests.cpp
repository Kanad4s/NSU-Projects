#include "../include/consoleParser.h"
#include "gtest/gtest.h"

using namespace consoleParser;

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