#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

class Menu {
public:
    Menu();
    ~Menu();

    void init(SDL_Renderer* renderer);
    void run();
    void navigate(int code);
    void renderMenu();

private:
    SDL_Renderer* m_renderer;
    TTF_Font* m_font;
    SDL_Color m_textColor;
    std::vector<std::string> m_options;
    int m_selectedIndex;


};