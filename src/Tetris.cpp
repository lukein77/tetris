#include "Tetris.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

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
            //std::cout << i << ", " << j << std::endl;
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
        checkLines();
        for (Figure *f : figures) f->checkDeadBlocks();
        addFigure();
    }
}

void Tetris::checkLines()
{
    int total_lines = 0;
    for (int i = GRID_HEIGHT-1; i >= 0; i--) {
        bool full_line = true;
        // check if line is full

        for (int j = 0; j < GRID_WIDTH; j++) {
            if (grid[i][j] == nullptr) {
                full_line = false;
                break;
            }
        }

        if (!full_line) {
            // if line is not full, move that line down as many times as lines filled previously
            if (total_lines > 0) {
                for (int j = 0; j < GRID_WIDTH; j++) {
                    if (grid[i][j]) grid[i][j]->setPosition(j, i+total_lines);  // update block position
                    grid[i+total_lines][j] = grid[i][j];        // update grid
                }
            }
        } else {
            // set alive = false to all blocks
            for (int j = 0; j < GRID_WIDTH; j++) {
                grid[i][j]->erase();
            }
            // count full line and move to the next
            total_lines++;  
        }
    }
    // add points
    if (total_lines > 0) {
        points += pow(2.0, total_lines-1) * 100;
    }
}

void Tetris::drawAll()
{
    renderer.drawGrid();
    for (Figure *f : figures) {
        f->draw(renderer);
    }
    renderer.renderText("TETRIS", 470, 50, FONTSIZE_LARGE, COLOR_YELLOW, true);
    renderer.renderText("SCORE", 470, 150, FONTSIZE_DEFAULT, COLOR_WHITE, true);
    renderer.renderText(std::to_string(points), 470, 200, FONTSIZE_DEFAULT, COLOR_WHITE, true);

    renderer.renderText("NEXT", 470, 300, FONTSIZE_DEFAULT, COLOR_WHITE, true);
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

