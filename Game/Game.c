#define MAXCARS 8
#define SPAWNRATE 0.5
#define WINDOWW 640
#define WINDOWH 360

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Car.h"

struct Game {
  struct Car* cars[MAXCARS];
  char carsSize;

  int camera;
  int iterator;
  bool end;

  struct Car* player1;
  struct Car* player2;
};

struct Game createGame() {
  struct Game game = {0};

  struct Car* player1 = (struct Car*)malloc(sizeof(struct Car));
  player1->rect.x = player1->rx = 100;
  player1->rect.y = player1->ry = 100;
  player1->rect.w = CARW;
  player1->rect.h = CARH;
  player1->speedx = 0.0;
  player1->speedy = 0.0;
  player1->up = true;
  player1->left = false;
  player1->right = false;
  player1->down = false;
  game.cars[game.carsSize] = player1;
  game.carsSize++;
  game.player1 = player1;

  struct Car* player2 = (struct Car*)malloc(sizeof(struct Car));
  player2->rect.x = player2->rx = 100;
  player2->rect.y = player2->ry = 200;
  player2->rect.w = CARW;
  player2->rect.h = CARH;
  player2->speedx = 0.0;
  player2->speedy = 0.0;
  player2->up = true;
  player2->left = false;
  player2->right = false;
  player2->down = false;
  game.cars[game.carsSize] = player2;
  game.carsSize++;
  game.player2 = player2;

  game.camera = 0;
  game.iterator = 0;
  game.end = 0;

  srand(time(NULL));

  return game;
}

void addCar(struct Game *game) {
  if (game->carsSize < MAXCARS) {
    int y = (rand() % 4) * 60 + 60 + 15;
    int x = 200 + game->camera + WINDOWW;
    double speed = (rand() % 130) + 120;
    struct Car* car = (struct Car*)malloc(sizeof(struct Car));
    car->rect.x = car->rx = x;
    car->rect.y = car->ry = y;
    car->rect.w = CARW;
    car->rect.h = CARH;
    car->speedx = speed;
    car->speedy = 0.0;
    car->up = true;
    car->left = false;
    car->right = false;
    car->down = false;
    game->cars[game->carsSize] = car;
    game->carsSize++;
  }
}

void removeCar(struct Game* game, int index) {
  free(game->cars[index]);
  for (game->iterator = index; game->iterator < game->carsSize - 1; game->iterator++)
    game->cars[game->iterator] = game->cars[game->iterator + 1];
  game->carsSize--;
}

void checkDeadCar(struct Game* game) {
  for(game->iterator = 2; game->iterator < game->carsSize; game->iterator++) {
    if (getMiddlePosition(*game->cars[game->iterator]).x - game->camera < 0) {
      removeCar(game, game->iterator);
      game->iterator--;
    }
  }
}

bool handleUpdateGame(struct Game* game, double deltaTime) {
  autoPilot(game->player1, *game->cars, game->carsSize);
  autoPilot(game->player2, *game->cars, game->carsSize);
  // if(carSpawnChrono.getDeltaTime() > SPAWNRATE) {
  //   Random::setRange(0, 1);
  //   if(Random::getRandomNumber() == 1)
  //     addCar();
  //   carSpawnChrono.reset();
  // }
  addCar(game);
  updatePlayer(game->player1, deltaTime);
  updatePlayer(game->player2, deltaTime);

  for(game->iterator = 0; game->iterator < game->carsSize; game->iterator++) {
    if (game->iterator >= 2)
      updateBot(game->cars[game->iterator], deltaTime);
    //gazon
    if (game->cars[game->iterator]->rect.y < 60 || game->cars[game->iterator]->rect.y + game->cars[game->iterator]->rect.h > 300) {
      game->cars[game->iterator]->speedx = game->cars[game->iterator]->speedx - game->cars[game->iterator]->speedx * 0.75 * deltaTime;
    }
  }

  checkDeadCar(game);

  //check end game
  if (getMiddlePosition(*game->player1).x - game->camera < -10 ||
      getMiddlePosition(*game->player2).x - game->camera < -10) {
      game->end = true;
  }

  //collision
  struct Car* car1;
  struct Car* car2;
  for(int i = 0; i < game->carsSize; i++) {
    for(game->iterator = i + 1; game->iterator < game->carsSize; game->iterator++) {
      car1 = game->cars[i];
      car2 = game->cars[game->iterator];
      if(intersection(car1->rect, car2->rect)) {
        collisionDetected(car1, car2);
        moveFromCollision(car1, car2);
      }
    }
  }

  //camera
  int windowW = WINDOWW;
  if (game->player1->rect.x - game->camera > 2*windowW/3 || game->player2->rect.x - game->camera > 2*windowW/3) {
    if (game->player1->rect.x >= game->player2->rect.x)
    game->camera += game->player1->rect.x - game->camera - 2*windowW/3;
    else
    game->camera += game->player2->rect.x - game->camera - 2*windowW/3;
  }

  return game->end; 
}