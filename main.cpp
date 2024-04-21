#include <iostream>
#include <stdio.h>
#include <typeinfo>
#include "game.cpp"

int main(int argc, char* args[]) {
    Game game;
    if (!game.init()) {
        std::cerr << "Failed to initialize the game!" << std::endl;
        return -1;
    }
    game.run();
    return 0;
}