#ifndef _DRAW_H_INCLUDED_
#define _DRAW_H_INCLUDED_

#include "defs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Renderer {
    private:
        SDL_Renderer *renderer;
        SDL_Window *window;
        TTF_Font *fonts[3];
    public:
        Renderer();
        ~Renderer();
        bool initialize();
        void clearScene();
        void renderScene();
        void drawSquare(int x, int y, SDL_Color color, bool fill=false);
        void drawGrid();
        void blit(Texture *texture, int x, int y);
        void renderText(std::string text, int x, int y, int size, SDL_Color color, bool centered=false);
        void renderGameOver();
};

#endif