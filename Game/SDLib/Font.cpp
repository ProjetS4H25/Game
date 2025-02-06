#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <string>

class Font {
private:
    TTF_Font* font;
    SDL_Color color;

public:
    Font(const char* fontPath, int fontSize, SDL_Color color = {255, 255, 255, 255}) {
        this->color = color;
        font = TTF_OpenFont(fontPath, fontSize);
    }

    ~Font() {
        if (font)
            TTF_CloseFont(font);
    }

    TTF_Font* getFont() const {
        return font;
    }

    SDL_Color getColor() const {
        return color;
    }
};
