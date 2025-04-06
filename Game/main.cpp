#include <iostream>
#include "SDLib/SDLib.cpp"

#include "GameC.cpp"

int main(int argc, char* argv[]) {
	SDLib::getInstance().createWindow("CarRace", 640, 360);
	SDLib::getInstance().createApp(new GameC());
	SDLib::getInstance().start();
	return 0;
}