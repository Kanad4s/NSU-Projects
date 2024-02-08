#include "../include/commandState.h"

using namespace commandState;

void CommandState::readCommand()
{
	std::string inputCommand;
	std::cin >> inputCommand;
	if (inputCommand == "tick") {
		_command = commands::tick;
	}
	else if (inputCommand == "dump") {
		_command = commands::dump;
	}
	else if (inputCommand == "help") {
		_command = commands::help;
	}
	else if (inputCommand == "exit") {
		_command = commands::exit;
	}
	else if (inputCommand == "tests") {
		_command = commands::tests;
	}
	else {
		_command = commands::notACommand;
	}
}

int CommandState::getCommand()
{
	return _command;
}
