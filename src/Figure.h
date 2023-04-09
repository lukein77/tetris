#ifndef _FIGURE_H_DEFINED_
#define _FIGURE_H_DEFINED_

#include "defs.h"
#include "Renderer.h"
#include "Block.h"
#include <vector>

class Figure {
    protected:
        position_t _pos;
        std::vector<Block*> _blocks;
        SDL_Color _color;
        bool _landing;
    public:
        Figure(position_t b1, position_t b2, position_t b3, position_t b4, SDL_Color color);
        ~Figure() {}
        void draw(Renderer &renderer);
        void rotate(bool **grid);
        void move(int dx, int dy, bool **grid);
        void update(bool **grid);
        bool isLanding() const { return _landing; }
        void setBlocks(bool **grid);
};

class Square : public Figure {
    public:
        Square() : Figure({0,0}, {0,-1}, {1,-1}, {1,0}, COLOR_RED) {}
};

class Line : public Figure {
    public:
        Line() : Figure({0,0}, {-2,0}, {-1,0}, {1,0}, COLOR_BLUE) {}
};

class JFigure : public Figure {
    public:
        JFigure() : Figure({0,0}, {0,-2}, {0,-1}, {-1,0}, COLOR_PURPLE) {}
};

class LFigure : public Figure {
    public:
        LFigure() : Figure({0,0}, {0,-2}, {0,-1}, {1,0}, COLOR_YELLOW) {}
};

class SFigure : public Figure {
    public:
        SFigure() : Figure({0,0}, {-1,0}, {0,-1}, {1,-1}, COLOR_LIGHTBLUE) {}
};

class ZFigure : public Figure {
    public:
        ZFigure() : Figure({0,0}, {-1,-1}, {0,-1}, {1,0}, COLOR_GREEN) {}
};

class TFigure : public Figure {
    public:
        TFigure() : Figure({0,0}, {-1,0}, {0,-1}, {1,0}, COLOR_ORANGE) {}
};


#endif  // _FIGURE_H_DEFINED_