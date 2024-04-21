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
const int GHOST_MOVE_STEP = 1;
const int ROUND_TIME = 60000;

struct Point {
    int x;
    int y;
};

Game::Game() : mWindow(nullptr), mRenderer(nullptr) {}

Game::~Game() {
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

std::vector<Point> generatePoints(int numPoints, int imgWidth, int imgHeight) {
    std::vector<Point> points;
    const int minDistance = 150; // Минимальное расстояние между точками

    for (int i = 0; i < numPoints; ++i) {
        int x, y;
        bool validPoint = false;

        // Генерация координат до тех пор, пока не будет найдена точка, удовлетворяющая условию
        while (!validPoint) {
            x = std::rand() % (WINDOW_WIDTH - imgWidth);
            y = std::rand() % (WINDOW_HEIGHT - imgHeight);

            validPoint = true; // Предполагаем, что точка допустима, и проверим это ниже

            // Проверяем новую точку на расстояние до уже существующих точек
            for (const auto& existingPoint : points) {
                int dx = x - existingPoint.x;
                int dy = y - existingPoint.y;
                if (std::sqrt(std::pow(dx, 2) + std::pow(dy, 2)) < minDistance) {
                    validPoint = false; // Если расстояние меньше minDistance, точка недопустима
                    break; // Прерываем цикл, чтобы не проверять остальные точки
                }
            }
        }

        // Добавляем допустимую точку в массив
        points.push_back({x, y});
    }
    return points;
}

void printPoints(const std::vector<Point>& points) {
    std::cout << "Array of coordinates:" << std::endl;
    for (const auto& point : points) {
        std::cout << "(" << point.x << ", " << point.y << ")" << std::endl;
    }
}

void Game::run() {
    std::srand(std::time(nullptr));

    bool quit = false;
    int numPoints = 5; // Number of points
    std::vector<Point> points = generatePoints(numPoints, ghost.width, ghost.height);
    size_t currentPointIndex = 0;
    int ghostX = 0;
    int ghostY = 0;
    int playerX = 400;
    int playerY = 400;
    int playerRotate = 0;
    bool animationStopped = false;

    printPoints(points);

    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();

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

        if (!animationStopped) {
            if (ghostX != points[currentPointIndex].x) {
                ghostX += (points[currentPointIndex].x - ghostX) > 0 ? GHOST_MOVE_STEP : -GHOST_MOVE_STEP;
            }
            if (ghostY != points[currentPointIndex].y) {
                ghostY += (points[currentPointIndex].y - ghostY) > 0 ? GHOST_MOVE_STEP : -GHOST_MOVE_STEP;
            }

            if (ghostX == points[currentPointIndex].x && ghostY == points[currentPointIndex].y) {
                currentPointIndex = (currentPointIndex + 1) % points.size();
            }
        }

        Uint32 currentTime = SDL_GetTicks();

        if (currentTime - startTime >= ROUND_TIME) { 
            animationStopped = true; 
        }

        SDL_Delay(10);

        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);

        player.render(mRenderer, playerX, playerY, playerRotate);
        ghost.render(mRenderer, ghostX, ghostY);

        SDL_RenderPresent(mRenderer);
    }
}