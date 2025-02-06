#pragma once
#include <map>
#include "SDL2/SDL_main.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "Font.cpp"
#include "Window.cpp"
#include "Renderer.cpp"
#include "Event.cpp"
#include "App.cpp"
#include "Chrono.cpp"

using namespace std;

class SDLib {
private:
  static SDLib instance;
  Renderer* renderer;
  Window* window;
  App* app;
  Chrono deltaTime;
  Chrono fpsChrono;
  bool end;
  bool warpMouse;
  int frameRate;
  int fpsCount;
  int fpsCountShow;
  bool showFPS;
  Font* fpsFont;

public:
  static SDLib& getInstance() {
    return instance;
  }

  SDLib() {
    TTF_Init();
    frameRate = -1;
    warpMouse = false;
    showFPS = false;
    fpsCount = 0;
    fpsCountShow = 0;
    fpsFont = new Font("SDLib/Font.ttf", 24);
  }

  ~SDLib() {
    delete window;
    delete renderer;
    delete app;
    delete fpsFont;
    TTF_Quit();
  }

  void createApp(App* app) {
    this->app = app;
  }

  void createWindow(const char* title, int w, int h) {
    this->window = new Window(title, w, h);
    this->renderer = new Renderer(this->window->getWindow());
  }

  void setFrameRate(int frameRate) {
    this->frameRate = frameRate;
  }

  void setShowFPS() {
    if (showFPS)
      showFPS = false;
    else
      showFPS = true;
  }

  void kill() {
    end = true;
  }

  void setWarpMouse(bool warpMouse) {
    this->warpMouse = warpMouse;
  }

  Renderer* getRenderer() {
    return renderer;
  }

  SDL_Rect getWindowSize() {
    return window->getWindowInfos();
  }

  void start() {
    end = false;
    deltaTime.reset();
    fpsChrono.reset();
    while(!end) {
      // Event handling
      while(Event::getPoll()) {
        switch(Event::getType()) {
          case SDL_QUIT:
            end = true;
            break;
          default:
            app->handleEvent();
            break;
        }
      }

      //Parametres
      if(warpMouse)
        window->warpMouse();

      //FrameRate
      if(frameRate != -1 && !end && (1.0 / double(frameRate) - deltaTime.getDeltaTime() > 0))
        SDL_Delay((1.0 / double(frameRate) - deltaTime.getDeltaTime()) * 1000);

      // Update
      app->handleUpdate(deltaTime.getDeltaTime());
      deltaTime.reset();

      //fps
      if (fpsChrono.getDeltaTime() < 1.0)
        fpsCount++;
      else {
        fpsChrono.reset();
        fpsCountShow = fpsCount;
        fpsCount = 0;
      }

      // Draw
      renderer->clear();
      app->handleDraw();

      //fps
      if (showFPS)
          renderer->drawText(*fpsFont, to_string(fpsCountShow), 10, 10);

      renderer->present();
    }
  }
};
SDLib SDLib::instance;