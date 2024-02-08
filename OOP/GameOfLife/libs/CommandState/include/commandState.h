#pragma once
#include <iostream>

namespace commandState {
	namespace commands {
		const int exit = 0;
		const int dump = 1;
		const int tick = 2;
		const int help = 3;
		const int notACommand = 4;
		const int tests = 5;
	}

	class CommandState
	{
	public:
		void readCommand();
		int getCommand();
	private:
		int _command;
	};
}


