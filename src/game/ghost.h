
#include <SDL2/SDL.h>
class Ghost{
    private:
        SDL_Texture* texture;
    public:
        Ghost();
        ~Ghost();
        
        bool isInited;
        int init(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer, int x, int y);
        int width;
        int height;
};