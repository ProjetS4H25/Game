#pragma once
#include "SDL2/SDL.h"

class Chrono {
private:
  unsigned int startTime;

public:
  Chrono() {
    reset();
  }

  void reset() {
    startTime = SDL_GetTicks64();
  }

  double getDeltaTime() {
    return (SDL_GetTicks64() - startTime)/1000.0;
  }
};