#ifndef _FIGURE_H_DEFINED_
#define _FIGURE_H_DEFINED_

#include "defs.h"
#include "DrawingManager.h"


class Figure {
    protected:
        position_t _pos;
        position_t _blocks[4];
        SDL_Color _color;
    public:
        Figure(position_t b1, position_t b2, position_t b3, position_t b4, SDL_Color color);
        ~Figure() {}
        void draw(DrawingManager &drawingManager);
        void rotate(bool clockwise);
};

class Square : public Figure {
    public:
        Square() : Figure({-1,-1}, {0,-1}, {-1,0}, {0,0}, COLOR_RED) {}
};

class Line : public Figure {
    public:
        Line() : Figure({-2,-1}, {-1,-1}, {0,-1}, {1,-1}, COLOR_BLUE) {}
};

#endif  // _FIGURE_H_DEFINED_