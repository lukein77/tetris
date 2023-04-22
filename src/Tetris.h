#ifndef _TETRIS_H_DEFINED_
#define _TETRIS_H_DEFINED_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "defs.h"
#include "Renderer.h"
#include "Audio.h"
#include "Figure.h"
#include <list>

enum State {
    PLAYING,
    PAUSE,
    GAME_OVER
};

class Tetris {
    private:
        Renderer renderer;
        Audio mixer;
        State _state;
        bool running;
        bool game_over;
        bool holding_down; 
        std::list<Figure*> figures;
        Figure *active_figure;
        Figure *next_figure;
        Block ***grid;
        int points;

        void handleEvents();
        void handleKeyPressed();
        void handleKeyUp(SDL_Scancode key);
        void drawAll();
        void updateAll();
        void addFigure();
        void checkLines();
        Figure *randomFigure();
    public:
        Tetris();
        ~Tetris();
        bool isRunning() { return running; }
        void mainLoop();
        void changeState(State state);
};


#endif