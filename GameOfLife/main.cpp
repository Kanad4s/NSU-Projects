#include <iostream>
#include "gtest/gtest.h"
#include "libs/Game/include/game.h"

//обернуть в неймспейс
//константы
//тесты отдельно
//мэин поправить

using namespace game;

int main(int argc, char* argv[])
{
	try {
		Game game;
		game.initialize(argc, argv);
		game.play();
	}
	catch (std::exception const& ex) {
		std::cout << ex.what() << std::endl;
	}
	return 0;
}