
#include <SDL2/SDL.h>


enum KEY_CODES {
    SPACE = 44,
    UP = 82,
    DOWN = 81,
    LEFT = 80,
    RIGHT = 79,
};
class Player{
    private:
        SDL_Texture* texture;
    public:
        Player();
        ~Player();
        
        bool isInited;
        int init(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer, int x, int y, int degreese);
        void move(SDL_Scancode keyCode, int& posX, int& posY, int step, int& rotate, int screenWidth, int screenHeight);
        int width;
        int height;
};