#include <stdio.h>
#include <stdbool.h>
#include "Rect.h"
#define CARW 45
#define CARH 30

struct Car {
  int i;
  struct Rect rect;
  double speedx;
  double speedy;
  double accelerationY;
  double accelerationX;
  double rx, ry;
};

struct Car Car(int x, int y) {

}

struct Rect getMiddlePosition(struct Car car) {
  return Rect(car.rect.x + car.rect.w / 2, car.rect.y + car.rect.h / 2, 0, 0);
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

void moveFromCollision(struct Car car1, struct Car car2) {
  struct Rect rear = {car1.rect.x + car1.rect.w / 4, car1.rect.y, car1.rect.w / 2, car1.rect.h / 8};
  struct Rect front = {car1.rect.x + car1.rect.w / 4, car1.rect.y + car1.rect.h - car1.rect.w / 8, car1.rect.w / 2, car1.rect.h / 8};
  struct Rect left = {car1.rect.x, car1.rect.y + car1.rect.h / 8, car1.rect.w / 4, car1.rect.h - car1.rect.h / 4};
  struct Rect right = {car1.rect.x + car1.rect.w - car1.rect.w / 4, car1.rect.y + car1.rect.h / 8, car1.rect.w / 4, car1.rect.h - car1.rect.h / 4};
  
  if (intersection(&front, &car2.rect))
    car1.rect.y = car1.ry = car2.rect.y - 2 - car2.rect.h;
  else if (intersection(&rear, &car2.rect))
    car1.rect.y = car1.ry = car2.rect.y + car1.rect.h + 2;
  else if (intersection(&left, &car2.rect))
    car1.rect.x = car1.rx = car2.rect.x + car2.rect.w + 2;
  else if (intersection(&right, &car2.rect))
    car1.rect.x = car1.rx = car2.rect.x - 2 - car1.rect.w;
}

bool intersection(struct Rect *A, struct Rect *B) {
  if ((A->x + A->w <= B->x) || (B->x + B->w <= A->x) ||
      (A->y + A->h <= B->y) || (B->y + B->h <= A->y)) {
      return false;
  }
  return true;
}


bool colisionDetection(struct Car car1, struct Car car2) {
  return SDL_HasIntersection(&car1.rect, &car2.rect);
}

void collisionDetected(struct Car *car1, struct Car *car2) {
  struct Rect rear = {car1->rect.x + car1->rect.w / 4, car1->rect.y, car1->rect.w / 2, car1->rect.h / 8};
  struct Rect front = {car1->rect.x + car1->rect.w / 4, car1->rect.y + car1->rect.h - car1->rect.w / 8, car1->rect.w / 2, car1->rect.h / 8};
  struct Rect left = {car1->rect.x, car1->rect.y + car1->rect.h / 8, car1->rect.w / 4, car1->rect.h - car1->rect.h / 4};
  struct Rect right = {car1->rect.x + car1->rect.w - car1->rect.w / 4, car1->rect.y + car1->rect.h / 8, car1->rect.w / 4, car1->rect.h - car1->rect.h / 4};
  
  if (intersection(&front, &car2)) {
      car1->speedy = 0.80 * car2->speedx;
  } else if (intersection(&rear, &car2)) {
      car1->speedy = 1.20 * car1->speedy;
  } else if (intersection(&left, &car2)) {
      car1->speedx = 90.0;
  } else if (intersection(&right, &car2)) {
      car1->speedx = -90.0;
  }
}

void updatePlayer(double deltaTime) {

}

void updateBot(struct Car *car, double deltatime) {
  car->rx += car->speedx * deltaTime;
  car->ry += car->speedy * deltaTime;
  car->rect.x = car->rx;
  car->rect.y = car->ry;

  if (car->rect.x < 0) {
    car->rect.x = car->rx = 0.0;
    car->speedx = 0.0;
  }
  else if (car->rect.x + car->rect.w > WINDOWW) {
    car->rect.x = car->rx = WINDOWW - car->rect.w;
    car->speedx = 0.0;
  }
}