#include "Game.h"
#include "item.cpp"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int PLAYER_MOVE_STEP = 10;
const int GHOST_MOVE_STEP = 1;
const int ROUND_TIME = 25000;
const int NUM_ITEMS = 9;

std::vector<FurnitureItem> items;

Game::Game() {}

Game::~Game() {
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

bool Game::init(SDL_Window* mWindow, SDL_Renderer* mRenderer) {
    ItemImages images[NUM_ITEMS];

    images[0] = {"assets/images/chair.png", "assets/images/fallen-chair.png"};
    images[1] = {"assets/images/chair.png", "assets/images/fallen-chair.png"};
    images[2] = {"assets/images/closet.png", "assets/images/closet-open.png"};
    images[3] = {"assets/images/chair.png", "assets/images/fallen-chair.png"};
    images[4] = {"assets/images/cat.png", "assets/images/cat-scared.png"};
    images[5] = {"assets/images/chair.png", "assets/images/fallen-chair.png"};
    images[6] = {"assets/images/table.png", "assets/images/table-fallen.png"};
    images[7] = {"assets/images/chair.png", "assets/images/fallen-chair.png"};
    images[8] = {"assets/images/closet.png", "assets/images/closet-open.png"};

    SDL_Surface* loadedSurface = IMG_Load("assets/images/bg.png");
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    this -> bgTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
    if (bgTexture == nullptr) {
        std::cerr << "Unable to create texture from image! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_FreeSurface(loadedSurface);

    ghost.init(mRenderer);
    player.init(mRenderer);

    for (int i = 0; i < NUM_ITEMS; ++i) {
        FurnitureItem item;
        item.item.init(mRenderer, images[i].normal, images[i].fallen);

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

void generatePoints(int NUM_ITEMS, int imgWidth, int imgHeight, int playerX, int playerY, int playerWidth, int playerHeight) {
    std::vector<Point> points;
    const int minDistance = 200;

    for (int i = 0; i < NUM_ITEMS; ++i) {
        int x, y;
        bool validPoint = false;

        while (!validPoint) {
            x = std::rand() % (WINDOW_WIDTH - imgWidth);
            y = std::rand() % (WINDOW_HEIGHT - imgHeight);

            if (std::abs(x - playerX) < minDistance && std::abs(y - playerY) < minDistance) {
                continue;
            }

            validPoint = true;

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

void Game::saveResult(Uint32 gameTime) {
    std::ifstream inFile("best-results.txt");
    if (!inFile.is_open()) {
        std::cout << "File not found, creating new one..." << std::endl;
        std::ofstream createFile("best-results.txt");
        createFile.close();
        inFile.open("best-results.txt");
    }

    std::vector<double> secondsVec;

    std::string line;
    while (std::getline(inFile, line)) {
        size_t dashPos = line.find('-');
        if (dashPos == std::string::npos) {
            std::cerr << "Invalid format: " << line << std::endl;
            continue;
        }

        size_t secPos = line.find("sec");
        if (secPos == std::string::npos) {
            std::cerr << "Invalid format: " << line << std::endl;
            continue;
        }

        std::string secondsStr = line.substr(dashPos + 2, secPos - dashPos - 3);

        // Конвертируем строку в double и добавляем в вектор
        double seconds = std::stod(secondsStr);
        secondsVec.push_back(seconds);
    }

    inFile.close();

    secondsVec.push_back(std::round(gameTime / 100) / 10.0);

    std::sort(secondsVec.rbegin(), secondsVec.rend());

    std::ofstream outFile("best-results.txt");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file for writing results." << std::endl;
        return;
    }

    for (size_t i = 0; i < std::min(secondsVec.size(), static_cast<size_t>(10)); ++i) {
        outFile << "Place " << i + 1 << " - " << secondsVec[i] << " sec" << std::endl;
    }

    outFile.close();
}

void Game::run(SDL_Window* mWindow, SDL_Renderer* mRenderer) {
    std::srand(std::time(nullptr));

    bool quit = false;
    size_t currentPointIndex = 0;
    int ghostX = 0;
    int ghostY = 0;
    int playerX= WINDOW_WIDTH - player.width - 10;
    int playerY =  WINDOW_HEIGHT - player.height - 10;
    int playerRotate = 0;
    bool animationStopped = false;
    generatePoints(items.size(), ghost.width, ghost.height, playerX, playerY, player.width, player.height);
    player.setItems(&items);
    this->level = 1;

    SDL_Event e;

    Uint32 roundStartTime = SDL_GetTicks();
    Uint32 gameStartTime = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if(e.type == SDL_KEYDOWN && !animationStopped){
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

            bool allDropped = true;
            for (auto it = items.begin(); it != items.end(); ++it) {
                if (!it->item.dropped) {
                    allDropped = false;
                    break;
                }
            }

            if (allDropped) {
                animationStopped = true;

                Uint32 gameEndTime = SDL_GetTicks();
                std::cout << "Game over. Total time: " << gameEndTime - gameStartTime << std::endl;
                this->saveResult(gameEndTime - gameStartTime);
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - roundStartTime >= ROUND_TIME) { 
            roundStartTime = SDL_GetTicks();
            this -> level += 1;
        }

        SDL_Delay(10);

        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);

        SDL_Rect renderRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        SDL_RenderCopy(mRenderer, this->bgTexture, NULL, &renderRect);

        player.render(mRenderer, playerX, playerY, playerRotate);
        ghost.render(mRenderer, ghostX, ghostY);

        for(int i = 0; i < items.size(); ++i){
            items[i].item.render(mRenderer, items[i].coords.x, items[i].coords.y);
        }
        
        SDL_RenderPresent(mRenderer);
    }
}

