#include "../include/consoleParser.h"

using namespace consoleParser;

ConsoleParser::ConsoleParser(int& iterations, std::string& inputFile, std::string& outputFile, int argc, char* argv[])
{
	for (int i = 1; i < argc; i++) {
		if (argv[i] == iterationsKey) {
			iterations = atoi(argv[i + 1]);
		}
		else if (argv[i] == outputFileKey) {
			outputFile = argv[i + 1];
		}
		else if (argv[i] == inputFileKey) {
			inputFile = argv[i + 1];
		}
	}
}
