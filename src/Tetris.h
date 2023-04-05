#ifndef _TETRIS_H_DEFINED_
#define _TETRIS_H_DEFINED_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "defs.h"
#include "Renderer.h"
#include "Figure.h"
#include <string>

class Tetris {
    private:
        Renderer renderer;
        bool running;
        Figure *fig;

        void handleEvents();
        void handleKeyDown(SDL_Scancode keycode);
    public:
        Tetris();
        ~Tetris();
        bool isRunning() { return running; }
        void mainLoop();
};


#endif