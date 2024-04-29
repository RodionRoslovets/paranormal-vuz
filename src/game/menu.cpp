#include "menu.h"
#include <iostream>

Menu::Menu() {
    m_renderer = nullptr;
    m_font = nullptr;
    m_selectedIndex = 0;
    m_textColor = {255, 255, 255, 255};
    m_highlightColor = {255, 0, 0, 255};
}

Menu::~Menu() {
    if (m_font != nullptr)
        TTF_CloseFont(m_font);
}

void Menu::init(SDL_Renderer* renderer) {
    m_renderer = renderer;
    m_font = TTF_OpenFont("Jacquard.ttf", 56); 

    m_options.push_back("Play");
    m_options.push_back("Quit");
}

bool Menu::run() {
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        if (m_selectedIndex == 0)
                            return true;
                        else if (m_selectedIndex == 1)
                            running = false;
                        break;
                    case SDLK_UP:
                        navigate(-1);
                        break;
                    case SDLK_DOWN:
                        navigate(1);
                        break;
                }
            }
        }

        renderMenu();
    }

    return false;
}

void Menu::navigate(int code) {
    m_selectedIndex += code;
    if (m_selectedIndex < 0)
        m_selectedIndex = m_options.size() - 1;
    else if (m_selectedIndex >= m_options.size())
        m_selectedIndex = 0;
}

void Menu::renderMenu() {
    SDL_RenderClear(m_renderer);

    int y = 300;

    for (size_t i = 0; i < m_options.size(); ++i) {
        SDL_Color textColor = (i == m_selectedIndex) ? m_highlightColor : m_textColor;

        SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_options[i].c_str(), textColor);
        if (!surface) {
            std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        if (!texture) {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

        SDL_Rect dstRect = {640 - texW / 2, y, texW, texH};
        SDL_RenderCopy(m_renderer, texture, NULL, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        y += texH + 10; 
    }

    SDL_RenderPresent(m_renderer);
}
