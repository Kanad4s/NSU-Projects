#include "../include/fileParser.h"

using namespace fileParser;
using namespace gameStateRegex;
using namespace message;

FileParser::FileParser(std::string inputFile, GameState& gameState)
{
	std::ifstream fin;
	fin.exceptions(std::ifstream::badbit);
	fin.open(inputFile.c_str(), std::ifstream::in);
	char readLine[readLineSize];
	fin.getline(readLine, readLineSize);
	Message message;
	if (!std::regex_search(readLine, gameStateRegex::fileFormat)) {
		message.errorFormatFile();
		fin.close();
		throw std::exception("Wrong input file format");
	}
	fin.getline(readLine, readLineSize);
	if (!fin.eof() && !gameState.setWorldName(readLine, worldName)) {
		if (!gameState.setRules(readLine, rules)) {
			message.errorNoRules();
			fin.close();
			throw std::exception("No rules in input file");
		}
	}
	else if (!fin.eof()) {
		fin.getline(readLine, readLineSize);
		if (!gameState.setRules(readLine, rules)) {
			message.errorNoRules();
			fin.close();
			throw std::exception("No rules in input file");
		}
	}
	fin.getline(readLine, readLineSize);
	if (!gameState.setSize(readLine, worldSize) && fin.eof()) {
		message.errorNoSize();
		fin.close();
		throw std::exception("No world size in input file");
	}
	bool isSetCells = false;
	while (!fin.eof()) {
		fin.getline(readLine, readLineSize);
		isSetCells |= gameState.setCells(readLine, coordinates);
	}
	if (!isSetCells) {
		message.errorNoCells();
		fin.close();
		throw std::exception("No start cells");
	}
	fin.close();
}
