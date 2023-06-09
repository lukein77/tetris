#include "Tetris.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

/// @brief Initializes the Renderer object and other variables, allocates memory for the grid and sets every position to nullptr.
Tetris::Tetris()
{
    running = renderer.initialize() & mixer.initialize();
    if (!running) return;
    
    srand(time(0));

    _state = PLAYING;
    holding_down = false;

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
}


/// @brief Deallocates memory and quits SDL.
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

/// @brief Main game loop. All logic and rendering functions are called from here.
void Tetris::mainLoop()
{
    addFigure();

    Uint64 handleInput_time = 0;
    Uint64 update_time = 0;
    
    while (running) {
        Uint64 start = SDL_GetTicks64();

        handleEvents();

        if (_state == PLAYING) {
            if (SDL_GetTicks64() >= handleInput_time + 50) {
                handleKeyPressed();
                handleInput_time = SDL_GetTicks64();
            }
            
            int update_interval = (holding_down) ? 0 : 250;
            if (SDL_GetTicks64() >= update_time + update_interval) {
                updateAll();
                update_time = SDL_GetTicks64();
            }
        }

        drawAll();

        // fps
        Uint64 timePassed = SDL_GetTicks64() - start;
        if (timePassed < 1000 / 60.0) {
            SDL_Delay(1000 / 60.0 - timePassed);
        }
    }
    if (_state == GAME_OVER) {
        // Make the player look at the Game Over message and think about their mistakes
        SDL_Delay(3000);
    }
}


/// @brief Handles user events such as closing the window or pressing a key.
void Tetris::handleEvents()
{
    SDL_Event event; 

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYUP:
                if (_state != GAME_OVER) {
                    handleKeyUp(event.key.keysym.scancode);
                }
                break;
            default:
                break;
        }
    }
}

/// @brief Handles keyboard input.
void Tetris::handleKeyPressed()
{
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);
    if (keyboard[SDL_SCANCODE_LEFT]) {
        active_figure->move(-1,0, grid);
    }
    else if (keyboard[SDL_SCANCODE_RIGHT]) {
        active_figure->move(1,0, grid);
    }

    holding_down = keyboard[SDL_SCANCODE_DOWN];
}

void Tetris::handleKeyUp(SDL_Scancode key)
{
    if (key == SDL_SCANCODE_ESCAPE) {
        _state = (_state == PLAYING) ? PAUSE : PLAYING;
    }
    else if (key == SDL_SCANCODE_UP) {
        if (_state == PLAYING)
            active_figure->rotate(grid);
    }
}

/// @brief Main game logic is done in this function. 
//         Updates the active figure and checks for lines if the figure lands, and checks for game over.
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
            SDL_Delay(250);
            addFigure();
        } else {
            // figure did not land, player loses
            std::cout << "GAME OVER\n";
            _state = GAME_OVER;
        }
    }
}

/// @brief Checks if one or more lines have been completed and updates the grid accordingly.
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
        mixer.playLineSound();
    } else {
        mixer.playLandSound();
    }
}

/// @brief Draws every element: background, figures and UI.
void Tetris::drawAll()
{
    renderer.clearScene();
    // Render figures
    renderer.drawGrid();
    for (Figure *f : figures) {
        f->draw(renderer);
    }
    // Render UI
    renderer.renderText("TETRIS", 480, 50, FONTSIZE_LARGE, COLOR_YELLOW, true);
    renderer.renderText("SCORE", 480, 150, FONTSIZE_DEFAULT, COLOR_WHITE, true);
    renderer.renderText(std::to_string(points), 480, 200, FONTSIZE_DEFAULT, COLOR_WHITE, true);
    renderer.renderText("NEXT", 480, 330, FONTSIZE_DEFAULT, COLOR_WHITE, true);
    next_figure->drawAsNext(renderer);
    
    if (_state == GAME_OVER) {
        renderer.renderMessage("GAME OVER");
        running = false;
    }
    else if (_state == PAUSE) {
        renderer.renderMessage("PAUSED");
    }

    renderer.renderScene();

}

/// @brief Calculates a random number between 0 and 6 and uses it to generate a random figure.
/// @return Pointer to a Figure object
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

/// @brief Adds a random figure and generates the next one.
void Tetris::addFigure()
{
    active_figure = next_figure;
    figures.push_front(active_figure);
    next_figure = randomFigure();
}

