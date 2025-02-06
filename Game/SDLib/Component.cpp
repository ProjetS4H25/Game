#pragma once
#include "SDLib.cpp"

class Component {
private:

public:
  virtual void update(double deltaTime) = 0;
  virtual void draw() = 0;
};