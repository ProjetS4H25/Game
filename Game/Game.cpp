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
      
      player1 = new Player(75, 100);
      player2 = new Player(255, 100);
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
      int x = Random::getRandomNumber() * 60 + 60 + 15;
      int y = 200 + camera + SDLib::getInstance().getWindowSize().h;
      Random::setRange(120, 250);
      double speed = double(Random::getRandomNumber());
      cars[carsSize] = new Bot(x, y);
      cars[carsSize]->setSpeed(0.0, speed);
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
      if (cars[iterator]->getMiddlePosition().y * -1 + camera + SDLib::getInstance().getWindowSize().h > SDLib::getInstance().getWindowSize().h + 10) {
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
      if (cars[iterator]->getRect().x < 60 || cars[iterator]->getRect().x + cars[iterator]->getRect().w > 300) {
        cars[iterator]->setSpeed(cars[iterator]->getSpeedx(), cars[iterator]->getSpeedy() - cars[iterator]->getSpeedy() * 0.75 * deltaTime);
      }
    }

    checkDeadCar();

    //check end game
    if (player1->getMiddlePosition().y * -1 + camera + SDLib::getInstance().getWindowSize().h > SDLib::getInstance().getWindowSize().h + 10 ||
        player2->getMiddlePosition().y * -1 + camera + SDLib::getInstance().getWindowSize().h > SDLib::getInstance().getWindowSize().h + 10) {
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
            car1->collisionDetected(car2->getRect(), car2->getSpeedy());
            car2->collisionDetected(car1->getRect(), car1->getSpeedy());
            car1->moveFromCollision(car2->getRect());
          }
      }
    }

    //camera
    int windowH = SDLib::getInstance().getWindowSize().h;
    if (windowH + camera - player1->getRect().y < windowH/3 || windowH + camera - player2->getRect().y < windowH/3) {
      if (-player1->getRect().y <= -player2->getRect().y)
        camera += -1 * (windowH + camera - player1->getRect().y - windowH/3);
      else
        camera += -1 * (windowH + camera - player2->getRect().y - windowH/3);
    }
      
  }

  void handleDraw() {
    //draw map
    SDLib::getInstance().getRenderer()->setDrawColor(0, 255, 0, 255);
    //grass
    for(iterator = 0; iterator < 13; iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({0 ,(iterator - 2) * 60 + (camera % 60), 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({300 ,(iterator - 2) * 60 + (camera % 60), 60, 60});
    }
    //road
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 255, 255);
    for(iterator = 0; iterator < 13; iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({60 ,(iterator - 2) * 60 + (camera % 60), 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({240 ,(iterator - 2) * 60 + (camera % 60), 60, 60});
    }
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 255, 255);
    for(iterator = 0; iterator < 13; iterator++) {
      SDLib::getInstance().getRenderer()->drawSquare({120 ,(iterator - 2) * 60 + (camera % 60), 60, 60});
      SDLib::getInstance().getRenderer()->drawSquare({180 ,(iterator - 2) * 60 + (camera % 60), 60, 60});
    }

    //draw NPCa
    SDLib::getInstance().getRenderer()->setDrawColor(255, 255, 0, 255);
    for(iterator = 2; iterator < carsSize; iterator++)
      cars[iterator]->drawOffset(SDLib::getInstance().getWindowSize().h + camera);

    //draw Player
    SDLib::getInstance().getRenderer()->setDrawColor(255, 0, 0, 255);
    player1->drawOffset(SDLib::getInstance().getWindowSize().h + camera);
    SDLib::getInstance().getRenderer()->drawText(*font, to_string(int(player1->getSpeedy())) + "km/h", 10, 60);
    SDLib::getInstance().getRenderer()->setDrawColor(0, 0, 255, 255);
    player2->drawOffset(SDLib::getInstance().getWindowSize().h + camera);
    SDLib::getInstance().getRenderer()->drawText(*font, to_string(int(player2->getSpeedy())) + "km/h", 190, 60);

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