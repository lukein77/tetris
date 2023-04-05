#include "Figure.h"

#include <stdio.h>

Figure::Figure(position_t b1, position_t b2, position_t b3, position_t b4, SDL_Color color) :
    _pos(INIT_X, INIT_Y),
    _blocks {
        Block(b1.x+INIT_X, b1.y+INIT_Y, color),
        Block(b2.x+INIT_X, b2.y+INIT_Y, color),
        Block(b3.x+INIT_X, b3.y+INIT_Y, color),
        Block(b4.x+INIT_X, b4.y+INIT_Y, color)
    }
{}

void Figure::draw(Renderer &renderer)
{
    for (int i = 0; i < 4; i++) {
        _blocks[i].draw(renderer);
    }
}

void Figure::rotate()
{
    position_t pivot = _blocks[0].getPosition();
    position_t rotated_pos[3];
    for (int i = 1; i < 4; i++) {
        rotated_pos[i-1] = _blocks[i].rotate(pivot);
    }
    bool collide = false;
    for (int i = 1; i < 4; i++) {
        if (_blocks[i].checkCollision(rotated_pos[i-1].x, rotated_pos[i-1].y)) {
            collide = true;
            break;
        }
    }
    if (!collide) {
        for (int i = 1; i < 4; i++) {
            _blocks[i].move(rotated_pos[i-1].x, rotated_pos[i-1].y);
        }
    }
}

void Figure::move(int dx, int dy)
{
    bool collide = false;
    for (int i = 0; i < 4; i++) {
        if (_blocks[i].checkCollision(dx, dy)) {
            collide = true;
            break;
        }
    }
    if (!collide) {
        for (int i = 0; i < 4; i++) {
            _blocks[i].move(dx, dy);
        }
        _pos.x += dx;
        _pos.y += dy;
    }
}

void Figure::update()
{
    move(0, 1);     // move down
    printf("%d %d\n", _pos.x, _pos.y);
}

