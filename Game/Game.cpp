#pragma once
#include "SDLib/TextureManager.cpp"
#include "SDLib/SDLib.cpp"
#include "SDLib/App.cpp"
#include "SDLib/Random.cpp"
#include "SDLib/Chrono.cpp"
#include "SDLib/Random.cpp"
#include "SDLib/Font.cpp"
#include <iostream>
#include <deque>

#include "Player.cpp"
#include "Bot.cpp"

#define MAXCARS 12
#define SPAWNRATE 0.5

class Game : public App {
private:
  Car* cars[MAXCARS];
  char carsSize;
  Chrono carSpawnChrono;
  bool debug;

  Font* font;
  int camera;
  int iterator;

  Player* player1;
  bool keyW;
  bool keyA;
  bool keyS;
  bool keyD;
  
  Player* player2;
  bool keyUp;
  bool keyDown;
  bool keyRight;
  bool keyLeft;

public:
  Game() {
      font = new Font("Font/Mario.ttf", 24);
      SDLib::getInstance().setFrameRate(60);

      camera = 0;
      
      player1 = new Player(100, 75);
      player2 = new Player(100, 255);
      keyW = false;
      keyA = false;
      keyS = false;
      keyD = false;
      keyUp = false;
      keyDown = false;
      keyRight = false;
      keyLeft = false;
      debug = false;

      cars[0] = player1;
      cars[1] = player2;
      carsSize = 2;
      carSpawnChrono.reset();
  }

  void handleEvent() {
    switch(Event::getType()) {
      case SDL_KEYDOWN:
        switch (Event::getKey()) {
          case SDLK_F1:
            SDLib::getInstance().setShowFPS();
            if(debug)
              debug = false;
            else
              debug = true;
            break;
          case SDLK_w:
            keyW = true;
            break;
          case SDLK_a:
            keyA = true;
            break;
          case SDLK_s:
            keyS = true;
            break;
          case SDLK_d:
            keyD = true;
            break;
          case SDLK_UP:
            keyUp = true;
            break;
          case SDLK_DOWN:
            keyDown = true;
            break;
          case SDLK_LEFT:
            keyLeft = true;
            break;
          case SDLK_RIGHT:
            keyRight = true;
            break;
        }
        break;

      case SDL_KEYUP:
        switch (Event::getKey()) {
          case SDLK_w:
            keyW = false;
            break;
          case SDLK_a:
            keyA = false;
            break;
          case SDLK_s:
            keyS = false;
            break;
          case SDLK_d:
            keyD = false;
            break;
          case SDLK_UP:
            keyUp = false;
            break;
          case SDLK_DOWN:
            keyDown = false;
            break;
          case SDLK_LEFT:
            keyLeft = false;
            break;
          case SDLK_RIGHT:
            keyRight = false;
            break;
        }
        break;
    }
  }

  void addCar() {
    if (carsSize < MAXCARS) {
      Random::setRange(0, 3);
      int y = Random::getRandomNumber() * 60 + 60 + 15;
      int x = 200 + camera + SDLib::getInstance().getWindowSize().w;
      Random::setRange(120, 250);
      double speed = double(Random::getRandomNumber());
      cars[carsSize] = new Bot(x, y);
      cars[carsSize]->setSpeed(speed, 0.0);
      carsSize++;
    }
  }

  void removeCar(char index) {
    delete cars[index];
    for (iterator = index; iterator < carsSize - 1; iterator++)
      cars[iterator] = cars[iterator + 1];
    carsSize--;
  }

  void checkDeadCar() {
    for(iterator = 2; iterator < carsSize; iterator++) {
      if (cars[iterator]->getMiddlePosition().x - camera < 0) {
        removeCar(iterator);
        iterator--;
      }
    }
  }

  void handleUpdate(double deltaTime) {
    player1->input(keyW, keyS, keyD, keyA);
    player2->input(keyUp, keyDown, keyRight, keyLeft);

    if(carSpawnChrono.getDeltaTime() > SPAWNRATE) {
      Random::setRange(0, 1);
      if(Random::getRandomNumber() == 1)
        addCar();
      carSpawnChrono.reset();
    }

    for(iterator = 0; iterator < carsSize; iterator++) {
      cars[iterator]->update(deltaTime);
      //grass
      if (cars[iterator]->getRect().y < 60 || cars[iterator]->getRect().y + cars[iterator]->getRect().h > 300) {
        cars[iterator]->setSpeed(cars[iterator]->getSpeedx() - cars[iterator]->getSpeedx() * 0.75 * deltaTime, cars[iterator]->getSpeedy());
      }
    }

    checkDeadCar();

    //check end game
    if (player1->getMiddlePosition().x - camera < -10 ||
        player2->getMiddlePosition().x - camera < -10) {
        SDLib::getInstance().kill();
    }

    //collision
    Car* car1;
    Car* car2;
    for(int i = 0; i < carsSize; i++) {
      for(iterator = i + 1; iterator < carsSize; iterator++) {
        car1 = cars[i];
        car2 = cars[iterator];
        if (car1->collisionDistance(car2->getMiddlePosition(), car2->getCollisionThreshold()));
          if(car1->colisionDetection(car2->getRect())) {
            car1->collisionDetected(car2->getRect(), car2->getSpeedx());
            car2->collisionDetected(car1->getRect(), car1->getSpeedx());
            car1->moveFromCollision(car2->getRect());
          }
      }
    }

    //camera
    int windowW = SDLib::getInstance().getWindowSize().w;
    if (player1->getRect().x - camera > 2*windowW/3 || player2->getRect().x - camera > 2*windowW/3) {
      if (player1->getRect().x >= player2->getRect().x)
        camera += player1->getRect().x - camera - 2*windowW/3;
      else
        camera += player2->getRect().x - camera - 2*windowW/3;
    }
      
  }

  void handleDraw() {
    //draw map
    SDLib::getInstance().getRenderer()->setDrawColor(0, 255, 0, 255);
    //grass
    for(iterator = 0; iterator < 13; iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({(iterator) * 60 - (camera % 60), 0, 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({(iterator) * 60 - (camera % 60), 300, 60, 60});
    }
    //road
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 255, 255);
    for(iterator = 0; iterator < 13; iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({(iterator) * 60 - (camera % 60), 60, 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({(iterator) * 60 - (camera % 60), 240, 60, 60});
    }
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 255, 255);
    for(iterator = 0; iterator < 13; iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({(iterator) * 60 - (camera % 60), 120, 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({(iterator) * 60 - (camera % 60), 180, 60, 60});
    }

    //draw NPCa
    SDLib::getInstance().getRenderer()->setDrawColor(255, 255, 0, 255);
    for(iterator = 2; iterator < carsSize; iterator++)
      cars[iterator]->drawOffset(camera);

    //draw Player
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 0, 255);
    player1->drawOffset(camera);
    SDLib::getInstance().getRenderer()->drawText(*font, to_string(int(player1->getSpeedx())) + "km/h", 10, 60);
    SDLib::getInstance().getRenderer()->setDrawColor(0, 0, 255, 255);
    player2->drawOffset(camera);
    SDLib::getInstance().getRenderer()->drawText(*font, to_string(int(player2->getSpeedx())) + "km/h", 190, 60);

    //debug
    if (debug) {
      SDLib::getInstance().getRenderer()->setDrawColor(255, 255, 255, 255);
      for(int i = 0; i < carsSize; i++)
        for(iterator = i + 1; iterator < carsSize; iterator++) {
          SDL_Point car1 = cars[i]->getMiddlePosition();
          SDL_Point car2 = cars[iterator]->getMiddlePosition();
          car1.y *= -1;
          car1.y += SDLib::getInstance().getWindowSize().h + camera + 45;
          car2.y *= -1;
          car2.y += SDLib::getInstance().getWindowSize().h + camera + 45;
          SDLib::getInstance().getRenderer()->drawline(car1, car2);
        }
    }

    //title
    SDLib::getInstance().getRenderer()->drawText(*font, "Car Race", 180, 50, true);
    SDLib::getInstance().getRenderer()->setDrawColor(0, 0, 0, 0);
  }
};