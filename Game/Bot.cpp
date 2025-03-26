#pragma once
#include "Car.cpp"

class Bot : public Car {
private:

public:
  Bot(int x, int y) : Car(x, y) {

  }

  void update(double deltaTime) {
    rx += speedx * deltaTime;
    ry += speedy * deltaTime;
    rect.x = rx;
    rect.y = ry;

    if (rect.y < 0) {
      rect.y = ry = 0.0;
      speedy = 0.0;
    }
    else if (rect.y + rect.h > SDLib::getInstance().getWindowSize().h) {
      rect.y = ry = SDLib::getInstance().getWindowSize().h - rect.h;
      speedy = 0.0;
    }
  }
};