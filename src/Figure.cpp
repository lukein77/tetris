#include "Figure.h"


Figure::Figure(position_t b1, position_t b2, position_t b3, position_t b4, SDL_Color color) :
    _pos(INIT_X, INIT_Y),
    _landing(false),
    _alive(true),
    _blocks {
        new Block(b1.x+INIT_X, b1.y+INIT_Y, color),
        new Block(b2.x+INIT_X, b2.y+INIT_Y, color),
        new Block(b3.x+INIT_X, b3.y+INIT_Y, color),
        new Block(b4.x+INIT_X, b4.y+INIT_Y, color)
    }
{}

Figure::~Figure()
{
    for (Block *b : _blocks) {
        delete b;
    }
    _blocks.clear();
}

void Figure::draw(Renderer &renderer)
{
    for (Block *b : _blocks) {
        b->draw(renderer);
    }
}

void Figure::rotate(Block ***grid)
{
    position_t pivot = _blocks.at(0)->getPosition();
    position_t rotated_pos[3];
    for (int i = 1; i < 4; i++) {
        rotated_pos[i-1] = _blocks.at(i)->rotate(pivot);
    }
    bool collide = false;
    for (int i = 1; i < 4; i++) {
        if (_blocks.at(i)->checkCollision(rotated_pos[i-1].x, rotated_pos[i-1].y, grid)) {
            collide = true;
            break;
        }
    }
    if (!collide) {
        for (int i = 1; i < 4; i++) {
            _blocks.at(i)->move(rotated_pos[i-1].x, rotated_pos[i-1].y);
        }
    }
}

void Figure::move(int dx, int dy, Block ***grid)
{
    bool collide = false;
    for (int i = 0; i < 4; i++) {
        if (_blocks.at(i)->checkCollision(dx, dy, grid)) {
            collide = true;
            break;
        }
    }
    if (!collide) {
        for (int i = 0; i < 4; i++) {
            _blocks.at(i)->move(dx, dy);
        }
        _pos.x += dx;
        _pos.y += dy;
    }
    else if (dy == 1) {
        _landing = true;
    }
}

void Figure::update(Block ***grid)
{
    move(0, 1, grid);     // move down
}

void Figure::checkDeadBlocks() {
    // check for 'dead' blocks and erase them
    auto it = _blocks.begin();
    while (it != _blocks.end()) {
        if (!(*it)->isAlive()) {
            Block *aux = *it;
            it = _blocks.erase(it);
            delete aux;
        } else {
            it++;
        }
    }
    if (_blocks.empty()) {
        _alive = false;
    }
}

void Figure::setBlocks(Block ***grid)
{
    for (Block *b : _blocks) {
        position_t pos = b->getPosition();
        grid[pos.y][pos.x] = b;
    }
}
