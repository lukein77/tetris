#ifndef _BLOCK_H_DEFINED_
#define _BLOCK_H_DEFINED_

#include "defs.h"
#include "Renderer.h"
#include <stdio.h>

class Block {
    private:
        int _x;
        int _y;
        Color _color;
        bool _alive;

    public:
        Block() {}
        Block(int x, int y, Color color) : _x(x), _y(y), _color(color), _alive(true) {}
        ~Block() {}
        bool checkCollision(int dx, int dy, Block ***grid);
        void move(int dx, int dy);
        void draw(Renderer &renderer);
        void drawAsNext(Renderer &renderer);
        void erase() { _alive = false; }
        void setPosition(int x, int y) { _x = x; _y = y;}
        position_t getPosition() const { return position_t(_x, _y); }
        int getX() const { return _x; }
        int getY() const { return _y; }
        position_t rotate(position_t pivot);
        const bool isAlive() const { return _alive; }
};

#endif