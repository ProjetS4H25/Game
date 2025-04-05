#pragma once
#include "Car.cpp"
#define WINDOWW 640
#define WINDOWH 360

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
    else if (rect.y + rect.h > WINDOWH) {
      rect.y = ry = WINDOWH - rect.h;
      speedy = 0.0;
    }
  }
};