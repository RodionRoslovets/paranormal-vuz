#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <typeinfo>
#include "game.cpp"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int IMAGE_WIDTH = 100;
const int IMAGE_HEIGHT = 100;
const int DEST_X = 400;
const int DEST_Y = 300;
const int MOVE_SPEED = 2;



int main(int argc, char* args[]) {
    // if (!init()) {
    //     std::cerr << "Failed to initialize!" << std::endl;
    //     return -1;
    // }

    // // if (!loadMedia()) {
    // //     std::cerr << "Failed to load media!" << std::endl;
    // //     return -1;
    // // }

    // bool quit = false;
    // SDL_Event e;
    // int imageX = 0;
    // int imageY = 0;

    // while (!quit) {
    //     while (SDL_PollEvent(&e) != 0) {
    //         if (e.type == SDL_QUIT) {
    //             quit = true;
    //         }

    //         if(e.type == SDL_KEYDOWN){
    //                 switch (e.key.keysym.scancode) {
    //                     case KEY_CODES::UP:
    //                         std::cout << "Up pressed"  << std::endl;
    //                         imageY -= MOVE_SPEED;
    //                         break;
    //                     case KEY_CODES::DOWN:
    //                         std::cout << "Down pressed"  << std::endl;
    //                         imageY += MOVE_SPEED;
    //                         break;
    //                     case KEY_CODES::LEFT:
    //                         std::cout << "Left pressed"  << std::endl;
    //                         imageX -= MOVE_SPEED;
    //                         break;
    //                     case KEY_CODES::RIGHT:
    //                         std::cout << "Right pressed"  << std::endl;
    //                         imageX += MOVE_SPEED;
    //                         break;
    //                     case KEY_CODES::SPACE:
    //                         std::cout << "Space pressed"  << std::endl;
    //                         break;
    //                     default:
    //                         break;
    //                 }
    //         }
    //     }

    //     SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //     SDL_RenderClear(gRenderer);

    //     SDL_Rect dstRect = {imageX, imageY, IMAGE_WIDTH, IMAGE_HEIGHT};
    //     SDL_RenderCopy(gRenderer, gTexture, nullptr, &dstRect);

    //     SDL_RenderPresent(gRenderer);
    // }

    // close();

    // return 0;

    Game game;
    if (!game.init()) {
        std::cerr << "Failed to initialize the game!" << std::endl;
        return -1;
    }
    game.run();
    return 0;
}