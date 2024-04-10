#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ghost.h>

Ghost::Ghost() : texture(nullptr) {}

Ghost::~Ghost() {
    SDL_DestroyTexture(texture);
}

int Ghost::init(SDL_Renderer* renderer){
    SDL_Surface* loadedSurface = IMG_Load("ghost.png");
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from image! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_FreeSurface(loadedSurface);

    this->isInited = true;

    return 0;
};

void Ghost::render(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect renderRect = { x, y, 100, 100 };
    SDL_RenderCopy(renderer, texture, NULL, &renderRect);
}