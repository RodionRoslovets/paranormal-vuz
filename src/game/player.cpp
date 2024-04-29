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
            rotate = -90;
            if(posY == 0){
                break;
            }

            for (auto it = this->begin(); it != this->end(); ++it) {
                if (posY - step + this->height >= it->coords.y && posY - step <= it->coords.y + it->item.height &&
                    posX + this->width > it->coords.x && posX < it->coords.x + it->item.width) {
                    return;
                }
            }

            posY -= step;
            break;
        case KEY_CODES::DOWN:
            rotate = 90;
            if(posY == screenHeight - this->height){
                break;
            }
            for (auto it = this->begin(); it != this->end(); ++it) {
                if (posY + step + this->height >= it->coords.y && posY + step <= it->coords.y + it->item.height &&
                    posX + this->width > it->coords.x && posX < it->coords.x + it->item.width) {
                    return; 
                }
            }
            posY += step;
            break;
        case KEY_CODES::LEFT:
            rotate = -180;
            if(posX == 0){
                break;
            }
            for (auto it = this->begin(); it != this->end(); ++it) {
                if (posX - step + this->width >= it->coords.x && posX - step <= it->coords.x + it->item.width &&
                    posY + this->height > it->coords.y && posY < it->coords.y + it->item.height) {
                    return; 
                }
            }
            posX -= step;
            break;
        case KEY_CODES::RIGHT:
            rotate = 0;
            if(posX == screenWidth - this->width){
                break;
            }
            for (auto it = this->begin(); it != this->end(); ++it) {
                if (posX + step + this->width >= it->coords.x && posX + step <= it->coords.x + it->item.width &&
                    posY + this->height > it->coords.y && posY < it->coords.y + it->item.height) {
                    return;
                }
            }
            posX += step;
            break;
        case KEY_CODES::SPACE:
            for (auto it = this->begin(); it != this->end(); ++it) {
                if (posX + step + this -> width < it -> coords.x || it -> coords.x + it -> item.width + step < posX)
                    continue;
                
                if (posY + step + this -> height < it -> coords.y || it -> coords.y + it -> item.height + step < posY)
                    continue;
                
                it -> item.dropped = !it -> item.dropped;
            }
            break;
        default:
            break;
    }
}
