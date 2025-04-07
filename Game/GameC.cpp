#pragma once
#include "SDLib/SDLib.cpp"
#include "SDLib/App.cpp"
#include "Game.h"
#include <stdio.h>
#include <stdlib.h>

class GameC : public App {
private:
  struct Game* game;

public:
  GameC() {
    game = (struct Game*)malloc(sizeof(struct Game));

    game->carsSize = 0;

    struct Car* player1 = (struct Car*)malloc(sizeof(struct Car));
    player1->rect.x = player1->rx = 100;
    player1->rect.y = player1->ry = 100;
    player1->rect.w = 45;
    player1->rect.h = 30;
    player1->speedx = 0.0;
    player1->speedy = 0.0;
    player1->up = true;
    player1->left = false;
    player1->right = false;
    player1->down = false;
    player1->accelerationX = 0.0;
    player1->accelerationY = 0.0;
    game->cars[game->carsSize] = player1;
    game->carsSize++;
    game->player1 = player1;

    struct Car* player2 = (struct Car*)malloc(sizeof(struct Car));
    player2->rect.x = player2->rx = 100;
    player2->rect.y = player2->ry = 200;
    player2->rect.w = 45;
    player2->rect.h = 30;
    player2->speedx = 0.0;
    player2->speedy = 0.0;
    player2->up = true;
    player2->left = false;
    player2->right = false;
    player2->down = false;
    player2->accelerationX = 0.0;
    player2->accelerationY = 0.0;
    game->cars[game->carsSize] = player2;
    game->carsSize++;
    game->player2 = player2;

    game->camera = 0;
    game->iterator = 0;
    game->end = false;

    srand(NULL);
  }

  void handleEvent() {

  }

  void handleUpdate(double deltaTime) {
    handleUpdateGame(game, deltaTime);
    if(game->end)
      SDLib::getInstance().kill();
  }

  void handleDraw() {
    //draw map
    SDLib::getInstance().getRenderer()->setDrawColor(0, 255, 0, 255);
    //grass
    for(game->iterator = 0; game->iterator <= 13; game->iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({(game->iterator) * 60 - (game->camera % 60), 0, 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({(game->iterator) * 60 - (game->camera % 60), 300, 60, 60});
    }
    //road
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 255, 255);
    for(game->iterator = 0; game->iterator <= 13; game->iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({(game->iterator) * 60 - (game->camera % 60), 60, 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({(game->iterator) * 60 - (game->camera % 60), 120, 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({(game->iterator) * 60 - (game->camera % 60), 180, 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({(game->iterator) * 60 - (game->camera % 60), 240, 60, 60});
    }

    //draw NPCa
    SDLib::getInstance().getRenderer()->setDrawColor(255, 255, 0, 255);
    for(game->iterator = 2; game->iterator <= game->carsSize - 1; game->iterator++)
      SDLib::getInstance().getRenderer()->drawSquare({game->cars[game->iterator]->rect.x - game->camera, game->cars[game->iterator]->rect.y, game->cars[game->iterator]->rect.w, game->cars[game->iterator]->rect.h});
    
    //draw Player
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 0, 255);
    SDLib::getInstance().getRenderer()->drawSquare({game->player1->rect.x - game->camera, game->player1->rect.y, game->player1->rect.w, game->player1->rect.h});
    SDLib::getInstance().getRenderer()->setDrawColor(0, 0, 255, 255);
    SDLib::getInstance().getRenderer()->drawSquare({game->player2->rect.x - game->camera, game->player2->rect.y, game->player2->rect.w, game->player2->rect.h});

    
    for(game->iterator = 0; game->iterator <= game->carsSize - 1; game->iterator++) {
      SDL_Rect left = {game->cars[game->iterator]->rect.x - game->camera + game->cars[game->iterator]->rect.w / 4, game->cars[game->iterator]->rect.y, game->cars[game->iterator]->rect.w / 2, game->cars[game->iterator]->rect.h / 4};
      SDL_Rect right = {game->cars[game->iterator]->rect.x - game->camera + game->cars[game->iterator]->rect.w / 4, game->cars[game->iterator]->rect.y + game->cars[game->iterator]->rect.h - game->cars[game->iterator]->rect.w / 6, game->cars[game->iterator]->rect.w / 2, game->cars[game->iterator]->rect.h / 4};
      SDL_Rect rear = {game->cars[game->iterator]->rect.x - game->camera, game->cars[game->iterator]->rect.y + game->cars[game->iterator]->rect.h / 4, game->cars[game->iterator]->rect.w / 4, game->cars[game->iterator]->rect.h - game->cars[game->iterator]->rect.h / 2};
      SDL_Rect front = {game->cars[game->iterator]->rect.x - game->camera + game->cars[game->iterator]->rect.w - game->cars[game->iterator]->rect.w / 4, game->cars[game->iterator]->rect.y + game->cars[game->iterator]->rect.h / 4, game->cars[game->iterator]->rect.w / 4, game->cars[game->iterator]->rect.h - game->cars[game->iterator]->rect.h / 2};
      SDLib::getInstance().getRenderer()->setDrawColor(255, 255, 255, 255);
      SDLib::getInstance().getRenderer()->drawSquare(rear);
      SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 0, 255);
      SDLib::getInstance().getRenderer()->drawSquare(front);
      SDLib::getInstance().getRenderer()->setDrawColor(0, 255, 0, 255);
      SDLib::getInstance().getRenderer()->drawSquare(left);
      SDLib::getInstance().getRenderer()->setDrawColor(0, 0, 255, 255);
      SDLib::getInstance().getRenderer()->drawSquare(right);
    }

    // //title
    // SDLib::getInstance().getRenderer()->drawText(*font, "Car Race", 180, 50, true);
    SDLib::getInstance().getRenderer()->setDrawColor(0, 0, 0, 0);
  }
};