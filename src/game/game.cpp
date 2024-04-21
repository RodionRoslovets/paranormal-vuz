#include "Game.h"

enum KEY_CODES {
    SPACE = 44,
    UP = 82,
    DOWN = 81,
    LEFT = 80,
    RIGHT = 79,
};

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int PLAYER_MOVE_STEP = 10;

Game::Game() : mWindow(nullptr), mRenderer(nullptr) {}

Game::~Game() {
    // SDL_DestroyTexture(ghostTexture);
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    IMG_Quit();
    SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_Image could not initialize! SDL_Image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    mWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == nullptr) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (mRenderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    ghost.init(mRenderer);
    player.init(mRenderer);

     if (!ghost.isInited || !player.isInited) {
        return false;
    }

    return true;
}

void Game::run() {
    bool quit = false;
    int ghostX = 0;
    int ghostY = 0;
    int playerX = 400;
    int playerY = 400;
    int playerRotate = 0;

    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if(e.type == SDL_KEYDOWN){
                switch (e.key.keysym.scancode) {
                    case KEY_CODES::UP:
                        std::cout << "Up pressed"  << std::endl;
                        if(playerY == 0){
                            break;
                        }
                        playerY -= PLAYER_MOVE_STEP;
                        playerRotate = -90;
                        break;
                    case KEY_CODES::DOWN:
                        std::cout << "Down pressed"  << std::endl;
                        if(playerY == WINDOW_HEIGHT - player.height){
                            break;
                        }
                        playerY += PLAYER_MOVE_STEP;
                        playerRotate = 90;
                        break;
                    case KEY_CODES::LEFT:
                        std::cout << "Left pressed"  << std::endl;
                        if(playerX == 0){
                            break;
                        }
                        playerX -= PLAYER_MOVE_STEP;
                        playerRotate = -180;
                        break;
                    case KEY_CODES::RIGHT:
                        std::cout << "Right pressed"  << std::endl;
                        if(playerX == WINDOW_WIDTH - player.width){
                            break;
                        }
                        playerX += PLAYER_MOVE_STEP;
                        playerRotate = 0;
                        break;
                    case KEY_CODES::SPACE:
                        std::cout << "Space pressed"  << std::endl;
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);

        // SDL_Rect dstRect = {0, 0, 100, 100};

        // SDL_RenderCopy(mRenderer, , NULL, &dstRect);
        ghost.render(mRenderer, ghostX, ghostY);
        player.render(mRenderer, playerX, playerY, playerRotate);

        SDL_RenderPresent(mRenderer);
    }
}