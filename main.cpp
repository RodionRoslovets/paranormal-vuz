#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include "game.cpp"

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_Image could not initialize! SDL_Image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF initialization failed: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* mWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == nullptr) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Renderer* mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (mRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    Game game;
    if (!game.init(mWindow,  mRenderer)) {
        std::cerr << "Failed to initialize the game!" << std::endl;
        return -1;
    }
    game.run(mWindow, mRenderer);

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    return 0;
}