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
    up = true;
    down = false;
    right = false;
    left = false;
  }

  void input(bool up, bool down, bool right, bool left) {
    this->up = up;
    this->down = down;
    this->right = right;
    this->left = left;
  }

  void bot(Car* cars[], char size) {
    char line = 0;
    char index = 2;
    for (int i = 2; i < size; i++) {
      if(cars[i]->getMiddlePosition().x != getMiddlePosition().x && cars[i]->getMiddlePosition().y != getMiddlePosition().y) {
        if(cars[i]->getMiddlePosition().y - getMiddlePosition().y <= cars[index]->getMiddlePosition().y - getMiddlePosition().y)
          index = i;
      }
    }
    if(size > 2) {
      bool decision = false;
      int deltaY = cars[index]->getMiddlePosition().y - getMiddlePosition().y;
      if(rect.y < 60) {
        input(true, false, true, false);
        decision = true;
      }
      else if(rect.y + rect.h > 300) {
        input(true, false, false, true);
        decision = true;
      }
      if(!decision && abs(deltaY < 30)) {
        if(rect.y < SDLib::getInstance().getWindowSize().h/2)
          input(true, false, true, false);
        else
          input(true, false, false, true);
      }
    }
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
    if (speedx + accelerationX * deltaTime <= MAXSPEED)
      speedx += accelerationX * deltaTime;
    else
      speedx = MAXSPEED;

    if (speedy + accelerationY * deltaTime <= MAXSPEED)
      speedy += accelerationY * deltaTime;
    else
      speedy = MAXSPEED;

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