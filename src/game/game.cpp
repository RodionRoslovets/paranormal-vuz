#include "Game.h"
#include "item.cpp"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int PLAYER_MOVE_STEP = 10;
const int GHOST_MOVE_STEP = 1;
const int ROUND_TIME = 60000;

std::vector<FurnitureItem> items;
int numPoints = 5;

Game::Game() : mWindow(nullptr), mRenderer(nullptr) {}

Game::~Game() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    IMG_Quit();
    TTF_Quit();
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

    if (TTF_Init() != 0) {
        std::cerr << "TTF initialization failed: " << TTF_GetError() << std::endl;
        return 1;
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

    for (int i = 0; i < numPoints; ++i) {
        FurnitureItem item;
        item.item.init(mRenderer);

        if(!item.item.isInited){
            std::cerr << "Item could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        item.coords.x = 0;
        item.coords.y = 0;

        items.push_back(item);
    }

    if (!ghost.isInited || !player.isInited) {
        return false;
    }

    return true;
}

void generatePoints(int numPoints, int imgWidth, int imgHeight) {
    std::vector<Point> points;
    const int minDistance = 150;

    for (int i = 0; i < numPoints; ++i) {
        int x, y;
        bool validPoint = false;

        // Генерация координат до тех пор, пока не будет найдена точка, удовлетворяющая условию
        while (!validPoint) {
            x = std::rand() % (WINDOW_WIDTH - imgWidth);
            y = std::rand() % (WINDOW_HEIGHT - imgHeight);

            validPoint = true;

            // Проверяем новую точку на расстояние до уже существующих точек
            for (const auto& existingPoint : points) {
                int dx = x - existingPoint.x;
                int dy = y - existingPoint.y;
                if (std::sqrt(std::pow(dx, 2) + std::pow(dy, 2)) < minDistance) {
                    validPoint = false; 
                    break; 
                }
            }
        }

        points.push_back({x, y});

        items[i].coords.x = x;
        items[i].coords.y = y;
    }
}

bool checkCollision(int x, int y, const FurnitureItem& target) {
    if ((x + target.item.width >= target.coords.x && x <= target.coords.x + target.item.width &&
        y +  target.item.height >= target.coords.y && y <= target.coords.y + target.item.height) ||
        (x == target.coords.x && y == target.coords.y)) {
        return true;
    }

    return false;
}

void Game::run() {
    std::srand(std::time(nullptr));

    bool quit = false;
    generatePoints(items.size(), ghost.width, ghost.height);
    size_t currentPointIndex = 0;
    int ghostX = 0;
    int ghostY = 0;
    int playerX= WINDOW_WIDTH - player.width - 10;
    int playerY =  WINDOW_HEIGHT - player.height - 10;
    int playerRotate = 0;
    bool animationStopped = false;
    player.setItems(&items);
    this->level = 1;

    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if(e.type == SDL_KEYDOWN){
                player.move(e.key.keysym.scancode, playerX, playerY, PLAYER_MOVE_STEP, playerRotate, WINDOW_WIDTH, WINDOW_HEIGHT);
            }
        }

        if (!animationStopped) {
            if (ghostX != items[currentPointIndex].coords.x) {
                ghostX += (items[currentPointIndex].coords.x - ghostX) > 0 ? GHOST_MOVE_STEP * this->level: -GHOST_MOVE_STEP * this->level;
            }
            if (ghostY != items[currentPointIndex].coords.y) {
                ghostY += (items[currentPointIndex].coords.y - ghostY) > 0 ? GHOST_MOVE_STEP * this->level : -GHOST_MOVE_STEP * this->level;
            }

            if (checkCollision(ghostX, ghostY, items[currentPointIndex])){
                items[currentPointIndex].item.dropped = true;
                currentPointIndex = (currentPointIndex + 1) % items.size();
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - startTime >= ROUND_TIME) { 
            // animationStopped = true;
            startTime = SDL_GetTicks();
            this -> level += 1;
        }

        SDL_Delay(10);

        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);

        player.render(mRenderer, playerX, playerY, playerRotate);
        ghost.render(mRenderer, ghostX, ghostY);

        for(int i = 0; i < items.size(); ++i){
            items[i].item.render(mRenderer, items[i].coords.x, items[i].coords.y);
        }

        SDL_RenderPresent(mRenderer);
    }
}