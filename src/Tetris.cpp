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
    grid = new Block**[GRID_HEIGHT];
    for (int i = 0; i < GRID_HEIGHT; i++) {
        grid[i] = new Block*[GRID_WIDTH];
        std::cout << i << std::endl;
    }
    
    // set every cell to nullptr
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            grid[i][j] = nullptr;
            std::cout << i << ", " << j << std::endl;
        }
    }
    

    points = 0;
}

Tetris::~Tetris()
{
    // deallocate memory for grid matrix
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
    
     
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            std::cout << i << ", " << j << ": " << grid[i][j] << std::endl;
        }
    }
    while (running) {
        Uint64 start = SDL_GetTicks64();

        handleEvents();

        if (framesPassed % 5 == 0) {
            handleInput();
        }

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
            default:
                break;
        }
    }
}

void Tetris::handleInput()
{
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    if (keyboard[SDL_SCANCODE_ESCAPE]) {
        running = false;
    }
    if (keyboard[SDL_SCANCODE_UP]) {
        active_figure->rotate(grid);
    }
    else if (keyboard[SDL_SCANCODE_LEFT]) {
        active_figure->move(-1,0, grid);
    }
    else if (keyboard[SDL_SCANCODE_RIGHT]) {
        active_figure->move(1,0, grid);
    }
    else if (keyboard[SDL_SCANCODE_DOWN]) {
        active_figure->move(0,1, grid);
    }
}

void Tetris::updateAll()
{
    active_figure->update(grid);
    if (active_figure->isLanding()) {
        active_figure->setBlocks(grid);
        addFigure();
    }
}

void Tetris::checkLines()
{
    int total_lines = 0;
    bool full_line = true;
    for (int i = GRID_HEIGHT-1; i >= 0; i--) {
        // check if line is full

        for (int j = 0; j < GRID_WIDTH; j++) {
            if (!grid[i][j]) {
                full_line = false;
                break;
            }
        }

        if (!full_line) {
            // if line is not full, move that line down as many times as lines filled previously
            if (total_lines > 0) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    grid[i-total_lines][j] = grid[i][j];
                }
            }
        } else {
            total_lines++;  // count full line and move to the next
        }
    }
}

void Tetris::drawAll()
{
    renderer.drawGrid();
    for (Figure *f : figures) {
        f->draw(renderer);
    }
}


void Tetris::addFigure()
{
    int random = rand() % 7;
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
        case 6:
            new_figure = new TFigure();
            break;
    }
    figures.push_front(new_figure);
    active_figure = figures.front();
}

