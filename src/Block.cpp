#include "Block.h"

void Block::move(int dx, int dy)
{
    _x += dx;
    _y += dy;
}

void Block::draw(Renderer &renderer)
{
    renderer.drawSquare(_x, _y, _color, true);
}

void Block::drawAsNext(Renderer &renderer)
{
    renderer.drawSquare(_x + NEXTFIGURE_OFFSET_X, _y + NEXTFIGURE_OFFSET_Y, _color, true);
}

position_t Block::rotate(position_t pivot)
{
    position_t new_pos(_x - pivot.x, _y - pivot.y);

    int aux = new_pos.x;
    new_pos.x = new_pos.y;
    new_pos.y = -aux;
    new_pos = new_pos + pivot;
    return position_t(new_pos.x - _x, new_pos.y - _y);
}

bool Block::checkCollision(int dx, int dy, Block ***grid)
{
    int new_x = _x + dx;
    int new_y = _y + dy;
    return ( ((new_x < 0 || new_x > GRID_WIDTH-1) || (new_y < 0 || new_y > GRID_HEIGHT-1)) || (grid[new_y][new_x] != nullptr) );
}
