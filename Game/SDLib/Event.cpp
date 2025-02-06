#pragma once
#include "SDL2/SDL.h"

class Event {
private:
  static SDL_Event sdlEvent;

public:
  static unsigned int getPoll() {
    return SDL_PollEvent(&sdlEvent);
  }
  static unsigned int getType() {
    return sdlEvent.type;
  }
  static unsigned int getKey() {
    return sdlEvent.key.keysym.sym;
  }
  static unsigned int getMouseClick() {
    return sdlEvent.button.button;
  }
  static SDL_Point getClickPos() {
    return {sdlEvent.button.x, sdlEvent.button.y};
  }
  static unsigned int getMouseWheelY() {
    return sdlEvent.wheel.y;
  }
  static SDL_Point getMouseWindowPos() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return {x, y};
  }
  static SDL_Point getMouseDeltaMotion() {
    return {sdlEvent.motion.xrel, sdlEvent.motion.yrel};
  }
  };
SDL_Event Event::sdlEvent;