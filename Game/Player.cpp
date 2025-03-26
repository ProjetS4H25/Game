#pragma once
#include "Car.cpp"

class Player : public Car {
private:
  bool up;
  bool down;
  bool right;
  bool left;

public:
  Player(int x, int y) : Car(x, y) {

  }

  void input(bool up, bool down, bool right, bool left) {
    this->up = up;
    this->down = down;
    this->right = right;
    this->left = left;
  }

  void update(double deltaTime) {
    //input modif
    if ((up && down) || (!up && !down))
      accelerationX = 0.0;
    else if (up) {
      if (accelerationX + ACCELERATION * deltaTime <= MAXACCELERATIONX)
        accelerationX += ACCELERATION * deltaTime;
      else
        accelerationX = MAXACCELERATIONX;
    }
    else if (down)
      accelerationX = -ACCELERATION * 2;
    if ((right && left) || (!right && !left))
      accelerationY = 0.0;
    else if (right)
      accelerationY = ACCELERATION * 3;
    else if (left)
      accelerationY = -ACCELERATION * 3;

    //movement
    if (speedy + accelerationX * deltaTime <= MAXSPEED)
      speedy += accelerationX * deltaTime;
    else
      speedy = MAXSPEED;

    if (speedx + accelerationY * deltaTime <= MAXSPEED)
      speedx += accelerationY * deltaTime;
    else
      speedx = MAXSPEED;

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