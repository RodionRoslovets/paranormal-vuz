#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <player.h>

Player::Player() : texture(nullptr) {}

Player::~Player() {
    SDL_DestroyTexture(texture);
}

int Player::init(SDL_Renderer* renderer){
    SDL_Surface* loadedSurface = IMG_Load("player.png");
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

void Player::render(SDL_Renderer* renderer, int x, int y, int degreese) {
    this->width = 100;
    this->height = 100;
    SDL_Rect renderRect = { x, y, this->width, this->height };
    // SDL_RenderCopy(renderer, texture, NULL, &renderRect);
    SDL_RenderCopyEx(renderer, texture, NULL, &renderRect, degreese, NULL, SDL_FLIP_NONE);
}