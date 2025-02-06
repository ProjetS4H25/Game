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
      accelerationY = 0.0;
    else if (up) {
      if (accelerationY + ACCELERATION * deltaTime <= MAXACCELERATIONY)
        accelerationY += ACCELERATION * deltaTime;
      else
        accelerationY = MAXACCELERATIONY;
    }
    else if (down)
      accelerationY = -ACCELERATION * 2;
    if ((right && left) || (!right && !left))
      accelerationX = 0.0;
    else if (right)
      accelerationX = ACCELERATION * 3;
    else if (left)
      accelerationX = -ACCELERATION * 3;

    //movement
    if (speedy + accelerationY * deltaTime <= MAXSPEED)
      speedy += accelerationY * deltaTime;
    else
      speedy = MAXSPEED;

    if (speedx + accelerationX * deltaTime <= MAXSPEED)
      speedx += accelerationX * deltaTime;
    else
      speedx = MAXSPEED;

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