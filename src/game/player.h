
#include <SDL2/SDL.h>
class Player{
    private:
        SDL_Texture* texture;
    public:
        Player();
        ~Player();
        
        bool isInited;
        int init(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer, int x, int y, int degreese);
        int width;
        int height;
};