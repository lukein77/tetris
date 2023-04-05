#include "Tetris.h"

Tetris::Tetris()
{
    if (renderer.initialize()) {
        running = true;
    } else {
        running = false;
    }
}

Tetris::~Tetris()
{
    SDL_Quit();
}

void Tetris::mainLoop()
{
    fig = new LFigure;
    unsigned int framesPassed = 0;
    while (running) {
        Uint64 start = SDL_GetTicks64();

        handleEvents();


        if (framesPassed % 12 == 0) {
            fig->update();
        }

        renderer.clearScene();

        // grid
        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                renderer.drawSquare(i, j, {0x5A, 0x5A, 0x5A, 0xFF});
            }
        }
        fig->draw(renderer);

        renderer.renderScene();

        // fps
        Uint64 timePassed = SDL_GetTicks64() - start;
        if (timePassed < 1000 / 60.0) {
            SDL_Delay(1000 / 60.0 - timePassed);
        }
        framesPassed++;
    }
}

void Tetris::handleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                handleKeyDown(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_LCTRL:
                        fig->rotate();
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void Tetris::handleKeyDown(SDL_Scancode keycode)
{
    switch (keycode) {
        case SDL_SCANCODE_ESCAPE:
            running = false;
            break;
        case SDL_SCANCODE_UP:
            fig->move(0,-1);
            break;
        case SDL_SCANCODE_DOWN:
            fig->move(0,1);
            break;
        case SDL_SCANCODE_LEFT:
            fig->move(-1,0);
            break;
        case SDL_SCANCODE_RIGHT:
            fig->move(1,0);
            break;
        default:
            break;
    }
}
