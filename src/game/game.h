#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <ghost.cpp>
#include <player.cpp>
#include <ctime>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <set>

class Game {
public:
    Game();
    ~Game();

    bool init(SDL_Window* mWindow, SDL_Renderer* mRenderer);
    void run(SDL_Window* mWindow, SDL_Renderer* mRenderer);

private:
    Ghost ghost;
    Player player;
    int level;
    void saveResult(Uint32 gameTime);
};