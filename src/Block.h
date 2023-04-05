#ifndef _BLOCK_H_DEFINED_
#define _BLOCK_H_DEFINED_

#include "defs.h"
#include "Renderer.h"
#include <stdio.h>

class Block {
    private:
        int _x;
        int _y;
        SDL_Color _color;

    public:
        Block() {}
        Block(int x, int y, SDL_Color color) : _x(x), _y(y), _color(color) {}
        ~Block() {}
        bool checkCollision(int dx, int dy);
        void move(int dx, int dy);
        void draw(Renderer &renderer);
        void setPosition(position_t pos) { _x = pos.x; _y = pos.y; }
        position_t getPosition() const { return position_t(_x, _y); }
        position_t rotate(position_t pivot);
};

#endif