#include "Tetris.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Tetris::Tetris()
{
    if (renderer.initialize()) {
        running = true;
    } else {
        running = false;
    }
    srand(time(0));

    // allocate memory for the grid
    grid = new bool*[GRID_HEIGHT];
    for (int i = 0; i < GRID_HEIGHT; i++) {
        grid[i] = new bool[GRID_WIDTH];
    }
}

Tetris::~Tetris()
{
    // deallocate grid
    for (int i = 0; i < GRID_HEIGHT; i++) {
        delete[] grid[i];
    }
    delete[] grid;

    delete active_figure;
    figures.clear();

    SDL_Quit();
}

void Tetris::mainLoop()
{
    addFigure();

    unsigned int framesPassed = 0;
    
    while (running) {
        Uint64 start = SDL_GetTicks64();

        handleEvents();


        if (framesPassed % 15 == 0) {
            updateAll();
        }

        renderer.clearScene();

        drawAll();

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
                        active_figure->rotate(grid);
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
            active_figure->rotate(grid);
            break;
        case SDL_SCANCODE_DOWN:
            active_figure->move(0,1, grid);
            break;
        case SDL_SCANCODE_LEFT:
            active_figure->move(-1,0, grid);
            break;
        case SDL_SCANCODE_RIGHT:
            active_figure->move(1,0, grid);
            break;
        default:
            break;
    }
}

void Tetris::updateAll()
{
    active_figure->update(grid);
    if (active_figure->isLanding()) {
        active_figure->setBlocks(grid);
        addFigure();
        std::cout << grid[5][19] << std::endl;
    }
}

void Tetris::addFigure()
{
    int random = rand() % 6;
    Figure *new_figure;
    switch (random) {
        case 0:
            new_figure = new Square();
            break;
        case 1:
            new_figure = new Line();
            break;
        case 2:
            new_figure = new JFigure();
            break;
        case 3:
            new_figure = new LFigure();
            break;
        case 4:
            new_figure = new SFigure();
            break;
        case 5:
            new_figure = new ZFigure();
            break;
    }
    figures.push_front(new_figure);
    active_figure = figures.front();
}

void Tetris::drawAll()
{
    renderer.drawGrid();
    for (Figure *f : figures) {
        f->draw(renderer);
    }
}

