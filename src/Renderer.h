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
        SDL_Texture *block_textures[7];
    public:
        Renderer();
        ~Renderer();
        bool initialize();
        void clearScene();
        void renderScene();
        void drawSquare(int x, int y, SDL_Color color, bool fill=false);
        void drawBlock(float x, float y, Color color);
        void drawGrid();
        void blit(Texture *texture, int x, int y);
        void renderText(std::string text, int x, int y, int size, SDL_Color color, bool centered=false);
        void renderGameOver();

        SDL_Texture *loadTexture(std::string);
        void renderTexture(SDL_Texture *texture, int x, int y);
};

#endif