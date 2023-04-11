#include "Tetris.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

Tetris::Tetris()
{
    if (renderer.initialize()) {
        running = true;
        
        srand(time(0));

        game_over = false;

        // allocate memory for the grid
        grid = new Block**[GRID_HEIGHT];
        for (int i = 0; i < GRID_HEIGHT; i++) {
            grid[i] = new Block*[GRID_WIDTH];
        }
        
        // set every cell to nullptr
        for (int i = 0; i < GRID_HEIGHT; i++) {
            for (int j = 0; j < GRID_WIDTH; j++) {
                grid[i][j] = nullptr;
            }
        }
        
        next_figure = randomFigure();
        points = 0;

    } else {
        running = false;
    }
}


Tetris::~Tetris()
{
    // deallocate memory for grid matrix
    for (int i = 0; i < GRID_HEIGHT; i++) {
        delete[] grid[i];
    }
    delete[] grid;

    delete next_figure;
    for (Figure *f : figures) {
        delete f;
    }
    figures.clear();

    SDL_Quit();
}

void Tetris::mainLoop()
{
    addFigure();

    unsigned int framesPassed = 0;
    Uint64 handleInput_time = 0;
    Uint64 update_time = 0;
    
    while (running) {
        Uint64 start = SDL_GetTicks64();

        handleEvents();

        if (SDL_GetTicks64() >= handleInput_time + 50) {
            handleInput();
            handleInput_time = SDL_GetTicks64();
        }

        if (SDL_GetTicks64() >= update_time + 200) {
            updateAll();
            update_time = SDL_GetTicks64();
        }

        drawAll();

        // fps
        Uint64 timePassed = SDL_GetTicks64() - start;
        if (timePassed < 1000 / 60.0) {
            SDL_Delay(1000 / 60.0 - timePassed);
        }
        framesPassed++;
    }
    if (game_over) {
        // Make the player look at the Game Over message and think about their mistakes
        SDL_Delay(3000);
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
            case SDL_KEYUP:
                if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
                    active_figure->rotate(grid);
                }
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
    if (keyboard[SDL_SCANCODE_LEFT]) {
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
    // update active figure
    active_figure->update(grid);
    // if figure lands, check for full lines
    if (active_figure->isLanding()) {
        if (active_figure->setBlocks(grid)) {
            checkLines();

            // remove 'dead' blocks and figures
            auto it = figures.begin();
            while (it != figures.end()) {
                (*it)->checkDeadBlocks();
                if (!(*it)->isAlive()) {
                    Figure *aux = *it;
                    it = figures.erase(it);
                    delete aux;
                } else {
                    it++;
                }
            }
            // add new figure
            addFigure();
        } else {
            // figure did not land, player loses
            std::cout << "GAME OVER\n";
            game_over = true;
        }
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
    renderer.clearScene();
    // Render figures
    renderer.drawGrid();
    for (Figure *f : figures) {
        f->draw(renderer);
    }
    // Render UI
    renderer.renderText("TETRIS", 470, 50, FONTSIZE_LARGE, COLOR_YELLOW, true);
    renderer.renderText("SCORE", 470, 150, FONTSIZE_DEFAULT, COLOR_WHITE, true);
    renderer.renderText(std::to_string(points), 470, 200, FONTSIZE_DEFAULT, COLOR_WHITE, true);
    renderer.renderText("NEXT", 470, 300, FONTSIZE_DEFAULT, COLOR_WHITE, true);
    next_figure->drawAsNext(renderer);
    
    if (game_over) {
        renderer.renderGameOver();
        running = false;
    }
    renderer.renderScene();

}

Figure *Tetris::randomFigure()
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
    return new_figure;
}

void Tetris::addFigure()
{
    active_figure = next_figure;
    figures.push_front(active_figure);
    next_figure = randomFigure();
}

