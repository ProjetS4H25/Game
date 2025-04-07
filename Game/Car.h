#ifndef CAR_H
#define CAR_H

#include "Car.c"

struct Car;
struct Car createCar(int x, int y);
struct Rect getMiddlePosition(struct Car car);
void moveFromCollision(struct Car* car1, struct Car* car2);
bool intersection(struct Rect A, struct Rect B);
void collisionDetected(struct Car *car1, struct Car *car2);
void updatePlayer(struct Car* car, double deltaTime);
void updateBot(struct Car* car, double deltaTime);
void autoPilot(struct Car* car, struct Car cars[], char size);
void input(struct Car* car, bool up, bool down, bool right, bool left);

#endif