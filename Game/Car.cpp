#pragma once
#include "SDLib/Component.cpp"

#define MAXSPEED 360
#define MAXACCELERATIONY 120
#define ACCELERATION 80.0

class Car : public Component {
protected:
    SDL_Rect rect;
    double speedx;
    double speedy;
    double accelerationY;
    double accelerationX;
    double rx, ry;

public:
    Car(int x, int y) {
      rect = {x, y, 30, 45};
      rx = x;
      ry = y;
      speedx = 0.0;
      speedy = 0.0;
      accelerationY = 0.0;
      accelerationX = 0.0;
    }

    SDL_Rect& getRect() {
        return rect;
    }

    SDL_Point getMiddlePosition() {
      return {rect.x + rect.w / 2, rect.y + rect.h / 2};
    }

    int getCollisionThreshold() {
      return rect.w * 2 / 3;
    }

    bool collisionDistance(SDL_Point& point2, int collisionThreshold2) {
      int distance = (getMiddlePosition().x - point2.x) * (getMiddlePosition().x - point2.x) + (getMiddlePosition().y - point2.y) * (getMiddlePosition().y - point2.y);
      return distance < getCollisionThreshold() || distance < collisionThreshold2;
    }
    
    void moveFromCollision(SDL_Rect& rect2) {
      SDL_Rect rear = {rect.x + rect.w / 4, rect.y, rect.w / 2, rect.h / 8};
      SDL_Rect front = {rect.x + rect.w / 4, rect.y + rect.h - rect.w / 8, rect.w / 2, rect.h / 8};
      SDL_Rect left = {rect.x, rect.y + rect.h / 8, rect.w / 4, rect.h - rect.h / 4};
      SDL_Rect right = {rect.x + rect.w - rect.w / 4, rect.y + rect.h / 8, rect.w / 4, rect.h - rect.h / 4};
      
      if (SDL_HasIntersection(&front, &rect2))
          rect.y = ry = rect2.y - 2 - rect2.h;
      else if (SDL_HasIntersection(&rear, &rect2))
          rect.y = ry = rect2.y + rect.h + 2;
      else if (SDL_HasIntersection(&left, &rect2))
          rect.x = rx = rect2.x + rect2.w + 2;
      else if (SDL_HasIntersection(&right, &rect2))
          rect.x = rx = rect2.x - 2 - rect.w;
    }

    bool colisionDetection(SDL_Rect& rect2) {
      return SDL_HasIntersection(&rect, &rect2);
      }

    void collisionDetected(SDL_Rect& rect2, double speed2) {
      SDL_Rect rear = {rect.x + rect.w / 4, rect.y, rect.w / 2, rect.h / 8};
      SDL_Rect front = {rect.x + rect.w / 4, rect.y + rect.h - rect.w / 8, rect.w / 2, rect.h / 8};
      SDL_Rect left = {rect.x, rect.y + rect.h / 8, rect.w / 4, rect.h - rect.h / 4};
      SDL_Rect right = {rect.x + rect.w - rect.w / 4, rect.y + rect.h / 8, rect.w / 4, rect.h - rect.h / 4};
      
      if (SDL_HasIntersection(&front, &rect2)) {
          speedy = 0.80 * speed2;
      } else if (SDL_HasIntersection(&rear, &rect2)) {
          speedy = 1.20 * speedy;
      } else if (SDL_HasIntersection(&left, &rect2)) {
          speedx = 90.0;
      } else if (SDL_HasIntersection(&right, &rect2)) {
          speedx = -90.0;
      }
    }

    virtual void update(double deltaTime) = 0;

    void setSpeed(double speedx, double speedy) {
        this->speedx = speedx;
        this->speedy = speedy;
    }

    double getSpeedx() {
      return speedx;
    }

    double getSpeedy() {
      return speedy;
    }

    void setAccelerationY(double accelerationY) {
        this->accelerationY = accelerationY;
    }

    double getAccelerationY() {
      return accelerationY;
    }

    void draw() {
        SDLib::getInstance().getRenderer()->drawSquare(rect);
    }

    void drawOffset(int offset) {
        SDLib::getInstance().getRenderer()->drawSquare({rect.x, offset - rect.y, rect.w, rect.h});
    }
};
