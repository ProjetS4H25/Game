#include <iostream>
#include "SDLib/SDLib.cpp"

#include "Game.cpp"

int main(int argc, char* argv[]) {
	SDLib::getInstance().createWindow("CarRace", 360, 640);
	SDLib::getInstance().createApp(new Game());
	SDLib::getInstance().start();
	return 0;
}