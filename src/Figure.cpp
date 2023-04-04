#include "Figure.h"

Figure::Figure(position_t b1, position_t b2, position_t b3, position_t b4, SDL_Color color) : _color(color), _pos(5,5)
{
    _blocks[0] = b1;
    _blocks[1] = b2;
    _blocks[2] = b3;
    _blocks[3] = b4;
}

void Figure::draw(Renderer &renderer)
{
    for (int i = 0; i < 4; i++) {
        renderer.drawSquare(_pos.x + _blocks[i].x, _pos.y + _blocks[i].y, _color, true);
    }
}

void Figure::rotate(bool clockwise)
{
    int aux;
    if (clockwise) {
        for (int i = 0; i < 4; i++) {
            aux = _blocks[i].x;
            _blocks[i].x = _blocks[i].y;
            _blocks[i].y = -aux;
        }
    } else {
        for (int i = 0; i < 4; i++) {
            aux = _blocks[i].x;
            _blocks[i].x = -_blocks[i].y;
            _blocks[i].y = aux;
        }
    }
}
