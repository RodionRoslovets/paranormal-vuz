
#include <SDL2/SDL.h>
class Item{
    private:
        SDL_Texture* texture;
        SDL_Texture* textureDropped;
    public:
        Item();
        
        bool isInited;
        int init(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer, int x, int y);
        int width;
        int height;
        bool dropped;
};