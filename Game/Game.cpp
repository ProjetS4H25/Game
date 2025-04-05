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
#include "Car.h"

#define MAXCARS 12
#define SPAWNRATE 0.5
#define WINDOWW 640
#define WINDOWH 360

class Game : public App {
private:
  Car* cars[MAXCARS];
  char carsSize;
  Chrono carSpawnChrono;

  Font* font;
  int camera;
  int iterator;

  Player* player1;
  Player* player2;

public:
  Game() {
      font = new Font("Font/Mario.ttf", 24);
      SDLib::getInstance().setFrameRate(60);

      camera = 0;
      
      player1 = new Player(100, 75);
      player2 = new Player(100, 255);

      cars[0] = player1;
      cars[1] = player2;
      carsSize = 2;
      carSpawnChrono.reset();
  }

  void handleEvent() {
    
  }

  void addCar() {
    if (carsSize < MAXCARS) {
      Random::setRange(0, 3);
      int y = Random::getRandomNumber() * 60 + 60 + 15;
      int x = 200 + camera + WINDOWW;
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
    player1->bot(cars, carsSize);
    player2->bot(cars, carsSize);
    if(carSpawnChrono.getDeltaTime() > SPAWNRATE) {
      Random::setRange(0, 1);
      if(Random::getRandomNumber() == 1)
        addCar();
      carSpawnChrono.reset();
    }

    for(iterator = 0; iterator < carsSize; iterator++) {
      cars[iterator]->update(deltaTime);
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
            car1->collisionDetected(car2->getRect(), car2->getSpeedx(), car2);
            car1->moveFromCollision(car2->getRect());
          }
      }
    }

    //camera
    int windowW = WINDOWW;
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

    //title
    SDLib::getInstance().getRenderer()->drawText(*font, "Car Race", 180, 50, true);
    SDLib::getInstance().getRenderer()->setDrawColor(0, 0, 0, 0);
  }
};