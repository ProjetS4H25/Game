#pragma once
#include "SDL2/SDL.h"

class Window {
private:
  SDL_Window* window;
  SDL_Rect windowInfos;

public:
  Window(const char* title, int w, int h) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    windowInfos.x = SDL_WINDOWPOS_CENTERED;
    windowInfos.y = SDL_WINDOWPOS_CENTERED;
    windowInfos.w = w;
    windowInfos.h = h;
  }
  ~Window() {
    SDL_DestroyWindow(window);
  }

  SDL_Window* getWindow() {
    return window;
  }
  void setWindowSize(int w, int h) {
    SDL_SetWindowSize(window, w, h);
    windowInfos.w = w;
    windowInfos.h = h;
  }
  void setWindowPosition(int x, int y) {
    SDL_SetWindowPosition(window, x, y);
    windowInfos.x = x;
    windowInfos.y = y;
  }
  SDL_Rect getWindowInfos() {
    return windowInfos;
  }
  void warpMouse() {
    SDL_WarpMouseInWindow(window, windowInfos.w/2, windowInfos.h/2);
  }
};