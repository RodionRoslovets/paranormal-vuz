#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <item.h>

Item::Item() : texture(nullptr), texture2(nullptr) {}

Item::~Item() {
    SDL_DestroyTexture(texture);
}

int Item::init(SDL_Renderer* renderer){
    SDL_Surface* itemSurface = IMG_Load("chair.png");
    if (itemSurface == nullptr) {
        std::cerr << "Unable to load image item! SDL_image Error: " << IMG_GetError() << std::endl;
        // return -1;
    }

    SDL_Surface* itemSurface2 = IMG_Load("fallen-chair.png");
    if (itemSurface2 == nullptr) {
        std::cerr << "Unable to load image item! SDL_image Error: " << IMG_GetError() << std::endl;
        // return -1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, itemSurface);
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from image! SDL Error: " << SDL_GetError() << std::endl;
        // return -1;
    }

    texture2 = SDL_CreateTextureFromSurface(renderer, itemSurface2);
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from image! SDL Error: " << SDL_GetError() << std::endl;
        // return -1;
    }

    SDL_FreeSurface(itemSurface);
    SDL_FreeSurface(itemSurface2);

    this->isInited = true;
    this->dropped = false;
    this->width = 100;
    this->height = 100;

    return 0;
};

void Item::render(SDL_Renderer* renderer, int x, int y) {
    SDL_Rect renderRect = { x, y, this->width, this->height };

    if(this->dropped){
        SDL_RenderCopy(renderer, texture2, NULL, &renderRect);
    } else {
        SDL_RenderCopy(renderer, texture, NULL, &renderRect);
    }
}