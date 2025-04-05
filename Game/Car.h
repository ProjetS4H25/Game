#ifndef CAR_H
#define CAR_H

#include "Car.c"

struct Car;
struct Car Car(int x, int y);
struct Rect getRect();
struct Rect getMiddlePosition(struct Car car);
int getCollisionThreshold();
bool collisionDistance(struct Rect point2, int collisionThreshold2); 
void moveFromCollision(struct Rect rect2);
bool colisionDetection(struct Rect rect2);
void collisionDetected(struct Rect rect2, double speed2);
void update(double deltaTime);
void setSpeed(double speedx, double speedy); 
double getSpeedx(); 
double getSpeedy(); 
void setAccelerationY(double accelerationY); 
double getAccelerationY();

#endif