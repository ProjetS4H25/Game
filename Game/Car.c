#include <stdio.h>
#include <stdbool.h>
#include "Rect.h"

#define CARW 45
#define CARH 30
#define WINDOWW 640
#define WINDOWH 360
#define ACCELERATION 80
#define MAXACCELERATIONX 200
#define MAXSPEED 360

struct Car {
  struct Rect rect;
  double speedx;
  double speedy;
  double accelerationY;
  double accelerationX;
  double rx, ry;
  bool up;
  bool down;
  bool right;
  bool left;
};

struct Car createCar(int x, int y) {
  struct Car car = {0};
  car.rx = car.rect.x = x;
  car.ry = car.rect.y = y;
  car.up = true;
  car.down = false;
  car.right = false;
  car.left = false;
  return car;
}

struct Rect getMiddlePosition(struct Car car) {
  return createRect(car.rect.x + car.rect.w / 2, car.rect.y + car.rect.h / 2, 0, 0);
}

int getCollisionThreshold() {
  return CARW * 2 / 3;
}

bool collisionDistance(struct Car car1, struct Car car2, int collisionThreshold2) {
  struct Rect mid1 = getMiddlePosition(car1);
  struct Rect mid2 = getMiddlePosition(car2);
  int distance = (mid1.x - mid2.x) * (mid1.x - mid2.x) + (mid1.y - mid2.y) * (mid1.y - mid2.y);
  return distance < getCollisionThreshold() || distance < collisionThreshold2;
}

bool intersection(struct Rect A, struct Rect B) {
  if ((A.x + A.w <= B.x) || (B.x + B.w <= A.x) ||
      (A.y + A.h <= B.y) || (B.y + B.h <= A.y)) {
      return false;
  }
  return true;
}

void moveFromCollision(struct Car* car1, struct Car* car2) {
  struct Rect left = createRect(car1->rect.x + car1->rect.w / 4, car1->rect.y, car1->rect.w / 2, car1->rect.h / 8);
  struct Rect right = createRect(car1->rect.x + car1->rect.w / 4, car1->rect.y + car1->rect.h - car1->rect.w / 8, car1->rect.w / 2, car1->rect.h / 8);
  struct Rect rear = createRect(car1->rect.x, car1->rect.y + car1->rect.h / 8, car1->rect.w / 4, car1->rect.h - car1->rect.h / 4);
  struct Rect front = createRect(car1->rect.x + car1->rect.w - car1->rect.w / 4, car1->rect.y + car1->rect.h / 8, car1->rect.w / 4, car1->rect.h - car1->rect.h / 4);

  if (intersection(front, car2->rect))
    car1->rect.x = car1->rx = car2->rect.x - 2 - car1->rect.w;
  else if (intersection(rear, car2->rect))
    car1->rect.x = car1->rx = car2->rect.x + car2->rect.w + 2;
  else if (intersection(left, car2->rect))
    car1->rect.y = car1->ry = car2->rect.y + car1->rect.h + 2;
  else if (intersection(right, car2->rect))
    car1->rect.y = car1->ry = car2->rect.y - 2 - car1->rect.h;
}

void collisionDetected(struct Car *car1, struct Car *car2) {
  struct Rect left = createRect(car1->rect.x + car1->rect.w / 4, car1->rect.y, car1->rect.w / 2, car1->rect.h / 8);
  struct Rect right = createRect(car1->rect.x + car1->rect.w / 4, car1->rect.y + car1->rect.h - car1->rect.w / 8, car1->rect.w / 2, car1->rect.h / 8);
  struct Rect rear = createRect(car1->rect.x, car1->rect.y + car1->rect.h / 8, car1->rect.w / 4, car1->rect.h - car1->rect.h / 4);
  struct Rect front = createRect(car1->rect.x + car1->rect.w - car1->rect.w / 4, car1->rect.y + car1->rect.h / 8, car1->rect.w / 4, car1->rect.h - car1->rect.h / 4);
  
  if (intersection(front, car2->rect)) {
      car1->speedy = 0.80 * car2->speedx;
  } else if (intersection(rear, car2->rect)) {
      car1->speedy = 1.20 * car1->speedy;
  } else if (intersection(left, car2->rect)) {
      car1->speedx = 90.0;
  } else if (intersection(right, car2->rect)) {
      car1->speedx = -90.0;
  }
}

void updatePlayer(struct Car *car, double deltaTime) {
  if ((car->up && car->down) || (!car->up && !car->down))
    car->accelerationX = 0.0;
  else if (car->up) {
    if (car->accelerationX + ACCELERATION * deltaTime <= MAXACCELERATIONX)
      car->accelerationX += ACCELERATION * deltaTime;
    else
      car->accelerationX = MAXACCELERATIONX;
  }
  else if (car->down)
    car->accelerationX = -ACCELERATION * 2;
  if ((car->right && car->left) || (!car->right && !car->left))
    car->accelerationY = 0.0;
  else if (car->right)
    car->accelerationY = ACCELERATION * 3;
  else if (car->left)
    car->accelerationY = -ACCELERATION * 3;

  //movement
  if (car->speedx + car->accelerationX * deltaTime <= MAXSPEED)
    car->speedx += car->accelerationX * deltaTime;
  else
    car->speedx = MAXSPEED;

  if (car->speedy + car->accelerationY * deltaTime <= MAXSPEED)
  car->speedy += car->accelerationY * deltaTime;
  else
    car->speedy = MAXSPEED;

  car->rx += car->speedx * deltaTime;
  car->ry += car->speedy * deltaTime;
  car->rect.x = car->rx;
  car->rect.y = car->ry;

  if (car->rect.y < 0) {
    car->rect.y = car->ry = 0.0;
    car->speedy = 0.0;
  }
  else if (car->rect.y + car->rect.h > WINDOWH) {
    car->rect.y = car->ry = WINDOWH - car->rect.h;
    car->speedy = 0.0;
  }
}

void updateBot(struct Car *car, double deltaTime) {
  car->rx += car->speedx * deltaTime;
  car->ry += car->speedy * deltaTime;
  car->rect.x = car->rx;
  car->rect.y = car->ry;

  if (car->rect.y < 0) {
    car->rect.y = car->ry = 0.0;
    car->speedy = 0.0;
  }
  else if (car->rect.y + car->rect.h > WINDOWH) {
    car->rect.y = car->ry = WINDOWH - car->rect.h;
    car->speedy = 0.0;
  }
}

void input(struct Car *car, bool up, bool down, bool right, bool left) {
  car->up = up;
  car->down = down;
  car->right = right;
  car->left = left;
}

void autoPilot(struct Car *car, struct Car cars[], char size) {
  char line = 0;
  char index = 2;
  for (int i = 2; i < size; i++) {
    if(getMiddlePosition(cars[i]).x != getMiddlePosition(*car).x && getMiddlePosition(cars[i]).y != getMiddlePosition(*car).y) {
      if(getMiddlePosition(cars[i]).y - getMiddlePosition(*car).y <= getMiddlePosition(cars[index]).y - getMiddlePosition(*car).y)
        index = i;
    }
  }
  if(size > 2) {
    bool decision = false;
    int deltaY = getMiddlePosition(cars[index]).y - getMiddlePosition(*car).y;
    if(car->rect.y < 60) {
      input(car, true, false, true, false);
      decision = true;
    }
    else if(car->rect.y + car->rect.h > 300) {
      input(car, true, false, false, true);
      decision = true;
    }
    if(!decision && abs(deltaY < 30)) {
      if(car->rect.y < WINDOWH/2)
        input(car, true, false, true, false);
      else
        input(car, true, false, false, true);
    }
  }
}