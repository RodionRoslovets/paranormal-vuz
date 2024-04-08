#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <typeinfo>
#include <ghost.cpp>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int IMAGE_WIDTH = 100;
const int IMAGE_HEIGHT = 100;
const int DEST_X = 400;
const int DEST_Y = 300;
const int MOVE_SPEED = 2;

enum KEY_CODES {
    SPACE = 44,
    UP = 82,
    DOWN = 81,
    LEFT = 80,
    RIGHT = 79,
};

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;

bool init() {
    Ghost ghost;
    ghost.hello();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    SDL_Surface* loadedSurface = IMG_Load("ghost.png");
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (gTexture == nullptr) {
        std::cerr << "Unable to create texture from image! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_FreeSurface(loadedSurface);

    return true;
}

void close() {
    SDL_DestroyTexture(gTexture);
    gTexture = nullptr;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadMedia()) {
        std::cerr << "Failed to load media!" << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;
    int imageX = 0;
    int imageY = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if(e.type == SDL_KEYDOWN){
                    switch (e.key.keysym.scancode) {
                        case KEY_CODES::UP:
                            std::cout << "Up pressed"  << std::endl;
                            imageY -= MOVE_SPEED;
                            break;
                        case KEY_CODES::DOWN:
                            std::cout << "Down pressed"  << std::endl;
                            imageY += MOVE_SPEED;
                            break;
                        case KEY_CODES::LEFT:
                            std::cout << "Left pressed"  << std::endl;
                            imageX -= MOVE_SPEED;
                            break;
                        case KEY_CODES::RIGHT:
                            std::cout << "Right pressed"  << std::endl;
                            imageX += MOVE_SPEED;
                            break;
                        case KEY_CODES::SPACE:
                            std::cout << "Space pressed"  << std::endl;
                            break;
                        default:
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_Rect dstRect = {imageX, imageY, IMAGE_WIDTH, IMAGE_HEIGHT};
        SDL_RenderCopy(gRenderer, gTexture, nullptr, &dstRect);

        SDL_RenderPresent(gRenderer);

    }

    close();

    return 0;
}