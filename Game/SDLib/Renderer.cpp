#pragma once
#include "Font.cpp"
#include "SDL2/SDL.h"

class Renderer {
private:
SDL_Renderer* renderer;

public:
  Renderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
  }
  ~Renderer() {
    SDL_DestroyRenderer(renderer);
  }

  void present() {
    SDL_RenderPresent(renderer);  
  }
  void clear() {
    SDL_RenderClear(renderer);
  }
  void drawline(const SDL_Point& point1, const SDL_Point& point2) {
    SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
  }
  void drawPoint(const SDL_Point& point) {
    SDL_RenderDrawPoint(renderer, point.x, point.y);
  }
  void drawSquare(const SDL_Rect& rect) {
    SDL_RenderDrawRect(renderer, &rect);
  }
  void copy(SDL_Texture* texture, const SDL_Rect& dest_rect) {
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
  }
  void setDrawColor(const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
  }
  SDL_Surface* createSurface(const char* filepath) {
    return IMG_Load(filepath);
  }
  SDL_Texture* createTexture(SDL_Surface* surface) {
    return SDL_CreateTextureFromSurface(renderer, surface);
  }
  SDL_Texture* createTexture(const char* filepath) {
    return SDL_CreateTextureFromSurface(renderer, IMG_Load(filepath));
  }

  void drawText(Font& font, const std::string& text, int x, int y, bool center = false) {
    if (!font.getFont()) 
      return;

    SDL_Surface* surface = TTF_RenderText_Blended(font.getFont(), text.c_str(), font.getColor());
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) 
      return;

    int textW, textH;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    int drawX = x;
    int drawY = y;

    if (center) {
        drawX = x - textW / 2;
        drawY = y - textH / 2;
    }

    SDL_Rect destRect = {drawX, drawY, textW, textH};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    SDL_DestroyTexture(texture);
}

};