#pragma once
#include <map>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDLib/SDLib.cpp"
#include <iostream>

class TextureManager {
private:
  static TextureManager instance;
  std::map<std::string, SDL_Texture*> textures;

public:
  static TextureManager& getInstance() {
    return instance;
  }

  ~TextureManager() {
    for (std::map<std::string, SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
        SDL_DestroyTexture(it->second);
    textures.clear();
}


  void addTexture(std::string key, const char* pathfile) {
    textures[key] = SDLib::getInstance().getRenderer()->createTexture(pathfile);
  }

  SDL_Texture* removeTexture(std::string key) {
    SDL_Texture* texture = textures[key];
    SDL_DestroyTexture(textures[key]);
    textures.erase(key);
    return texture;
  }

  SDL_Texture* getTexture(std::string key) {
    return textures[key];
  }
};
TextureManager TextureManager::instance;