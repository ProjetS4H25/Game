#ifndef GAME_H
#define GAME_H

#include "Game.c"

struct Game;
struct Game createGame();
void addCar(struct Game *game, struct Car* cars[]);
void removeCar(struct Game* game, int index);
void checkDeadCar(struct Game* game);
bool handleUpdateGame(struct Game* game, double deltaTime);

#endif