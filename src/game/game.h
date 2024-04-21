#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <ghost.cpp>
#include <player.cpp>
#include <ctime>
#include <vector>
#include <cmath>

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();

private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Ghost ghost;
    Player player;
};