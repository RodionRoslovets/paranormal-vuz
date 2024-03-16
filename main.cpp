#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <typeinfo>

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720

enum KEY_CODES {
    SPACE = 44,
    UP = 82,
    DOWN = 81,
    LEFT = 80,
    RIGHT = 79,
};

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Paranormal", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    bool running = true;
    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case KEY_CODES::UP:
                            std::cout << "Up pressed"  << std::endl;
                            break;
                        case KEY_CODES::DOWN:
                            std::cout << "Down pressed"  << std::endl;
                            break;
                        case KEY_CODES::LEFT:
                            std::cout << "Left pressed"  << std::endl;
                            break;
                        case KEY_CODES::RIGHT:
                            std::cout << "Right pressed"  << std::endl;
                            break;
                        case KEY_CODES::SPACE:
                            std::cout << "Space pressed"  << std::endl;
                            break;
                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    return 0;
}