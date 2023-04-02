#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "defs.h"
#include "DrawingManager.h"
#include "Figure.h"
#include <string>

DrawingManager drawingManager;

int main(int argc, char *argv[]) {

    bool running = false;

    if (drawingManager.initialize()) {
        running = true;
    } else {
        return 1;
    }
    
    SDL_Event event;
    long framesPassed = 0;

    Figure *line = new Line;
    Figure *square = new Square;

    while (running) {
        
        Uint64 start = SDL_GetTicks64();

        // events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            running = false;
                            break;
                        case SDL_SCANCODE_LCTRL:
                            line->rotate(false);
                            break;
                        case SDL_SCANCODE_RCTRL:
                            line->rotate(true);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        drawingManager.clearScene();

        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                drawingManager.drawSquare(i, j, {0x5A, 0x5A, 0x5A, 0xFF});
            }
        }
        
        line->draw(drawingManager);
        square->draw(drawingManager);

        drawingManager.renderScene();


        Uint64 timePassed = SDL_GetTicks64() - start;
        if (timePassed < 1000 / 60.0) {
            SDL_Delay(1000 / 60.0 - timePassed);
        }

        framesPassed++;

    }
    
    SDL_Quit();
    return 0;
}