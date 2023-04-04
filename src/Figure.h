#ifndef _FIGURE_H_DEFINED_
#define _FIGURE_H_DEFINED_

#include "defs.h"
#include "Renderer.h"


class Figure {
    protected:
        position_t _pos;
        position_t _blocks[4];
        SDL_Color _color;
    public:
        Figure(position_t b1, position_t b2, position_t b3, position_t b4, SDL_Color color);
        ~Figure() {}
        void draw(Renderer &renderer);
        void rotate(bool clockwise);
};

class Square : public Figure {
    public:
        Square() : Figure({0,0}, {0,-1}, {1,-1}, {1,0}, COLOR_RED) {}
};

class Line : public Figure {
    public:
        Line() : Figure({-2,0}, {-1,0}, {0,0}, {1,0}, COLOR_BLUE) {}
};

class JFigure : public Figure {
    public:
        JFigure() : Figure({0,-2}, {0,-1}, {0,0}, {-1,0}, COLOR_BLUE) {}
};

class LFigure : public Figure {
    public:
        LFigure() : Figure({0,-2}, {0,-1}, {0,0}, {1,0}, COLOR_BLUE) {}
};

class SFigure : public Figure {
    public:
        SFigure() : Figure({-1,0}, {0,0}, {0,-1}, {1,-1}, COLOR_BLUE) {}
};

class ZFigure : public Figure {
    public:
        ZFigure() : Figure({-1,-1}, {0,-1}, {0,0}, {1,0}, COLOR_BLUE) {}
};


#endif  // _FIGURE_H_DEFINED_