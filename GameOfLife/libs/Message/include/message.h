#pragma once
#include <iostream>

namespace message {
	class Message
	{
	public:
		void start() const;
		void help() const;
		void errorFormatFile() const;
		void errorNoRules() const;
		void errorNoCells() const;
		void errorNoSize() const;
		void unknownCommand() const;
		void fileSaved(std::string outputFile) const;
		void end() const;
	};
}

