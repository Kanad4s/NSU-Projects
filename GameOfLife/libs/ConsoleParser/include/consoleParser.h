#pragma once
#include <string>

namespace consoleParser {
	const std::string iterationsKey("-i");
	const std::string inputFileKey("-file");
	const std::string outputFileKey("-o");

	class ConsoleParser
	{
	public:
		ConsoleParser(int& iterations, std::string& inputFile, std::string& outputFile, int argc, char* argv[]);
	};
}


