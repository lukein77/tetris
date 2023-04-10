#ifndef _TETRIS_H_DEFINED_
#define _TETRIS_H_DEFINED_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "defs.h"
#include "Renderer.h"
#include "Figure.h"
#include <list>

class Tetris {
    private:
        Renderer renderer;
        bool running;
        std::list<Figure*> figures;
        Figure* active_figure;
        Block ***grid;
        int points;

        void handleEvents();
        void handleInput();
        void drawAll();
        void updateAll();
        void addFigure();
        void checkLines();
    public:
        Tetris();
        ~Tetris();
        bool isRunning() { return running; }
        void mainLoop();
};


#endif