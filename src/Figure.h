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
        Color _color;
        bool _landing;
        bool _alive;
    public:
        Figure(position_t b1, position_t b2, position_t b3, position_t b4, Color color);
        ~Figure();
        void draw(Renderer &renderer);
        void drawAsNext(Renderer &renderer);
        void rotate(Block ***grid);
        void move(int dx, int dy, Block ***grid);
        void update(Block ***grid);
        void checkDeadBlocks();
        bool isLanding() const { return _landing; }
        bool isAlive() const { return _alive; }
        bool setBlocks(Block ***grid);
};

class Square : public Figure {
    public:
        Square() : Figure({0,0}, {0,-1}, {-1,-1}, {-1,0}, Color::RED) {}
};

class Line : public Figure {
    public:
        Line() : Figure({0,-1}, {-2,-1}, {-1,-1}, {1,-1}, Color::BLUE) {}
};

class JFigure : public Figure {
    public:
        JFigure() : Figure({0,0}, {0,-2}, {0,-1}, {-1,0}, Color::PURPLE) {}
};

class LFigure : public Figure {
    public:
        LFigure() : Figure({0,0}, {0,-2}, {0,-1}, {1,0}, Color::YELLOW) {}
};

class SFigure : public Figure {
    public:
        SFigure() : Figure({0,0}, {-1,0}, {0,-1}, {1,-1}, Color::LIGHT_BLUE) {}
};

class ZFigure : public Figure {
    public:
        ZFigure() : Figure({0,0}, {-1,-1}, {0,-1}, {1,0}, Color::GREEN) {}
};

class TFigure : public Figure {
    public:
        TFigure() : Figure({0,0}, {-1,0}, {0,-1}, {1,0}, Color::ORANGE) {}
};


#endif  // _FIGURE_H_DEFINED_