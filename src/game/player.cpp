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
    this->width = 100;
    this->height = 100;

    return 0;
};

void Player::render(SDL_Renderer* renderer, int x, int y, int degreese) {
    SDL_Rect renderRect = { x, y, this->width, this->height };
    SDL_RenderCopyEx(renderer, texture, NULL, &renderRect, degreese, NULL, SDL_FLIP_NONE);
}

void Player::move(SDL_Scancode keyCode, int& posX, int& posY, int step, int& rotate, int screenWidth, int screenHeight) {

    switch (keyCode) {
        case KEY_CODES::UP:
            std::cout << "Up pressed"  << std::endl;
            if(posY == 0){
                break;
            }
            posY -= step;
            rotate = -90;
            break;
        case KEY_CODES::DOWN:
            std::cout << "Down pressed"  << std::endl;
            if(posY == screenHeight - this->height){
                break;
            }
            posY += step;
            rotate = 90;
            break;
        case KEY_CODES::LEFT:
            std::cout << "Left pressed"  << std::endl;
            if(posX == 0){
                break;
            }
            posX -= step;
            rotate = -180;
            break;
        case KEY_CODES::RIGHT:
            std::cout << "Right pressed"  << std::endl;
            if(posX == screenWidth - this->width){
                break;
            }
            posX += step;
            rotate = 0;
            break;
        case KEY_CODES::SPACE:
             for (auto it = this->begin(); it != this->end(); ++it) {
                std::cout << "Координаты: (" << it->coords.x  << ", " << it->coords.y << ")" << std::endl;

                if (posX + 100 < it -> coords.x || it -> coords.x + 100 < posX)
                    continue;
                
                if (posY + 100 < it -> coords.y || it -> coords.y + 100 < posY)
                    continue;
                
                it -> item.dropped = false;
            }

            break;
        default:
            break;
    }
}
