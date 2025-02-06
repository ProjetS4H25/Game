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

    if (rect.x < 0) {
      rect.x = rx = 0.0;
      speedx = 0.0;
    }
    else if (rect.x + rect.w > SDLib::getInstance().getWindowSize().w) {
      rect.x = rx = SDLib::getInstance().getWindowSize().w - rect.w;
      speedx = 0.0;
    }
  }
};