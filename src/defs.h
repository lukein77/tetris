#ifndef _DEFS_H_DEFINED_
#define _DEFS_H_DEFINED_

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 663

#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define BLOCK_SIZE 36
#define INIT_X 5
#define INIT_Y 1
#define NEXTFIGURE_OFFSET_X 9.3
#define NEXTFIGURE_OFFSET_Y 12

#define COLOR_WHITE     {0xFF, 0xFF, 0xFF, 0xFF}
#define COLOR_RED       {0xF2, 0x00, 0x00, 0xFF}
#define COLOR_BLUE      {0x34, 0x8F, 0xEB, 0xFF}
#define COLOR_LIGHTBLUE {0x1D, 0xCD, 0xE0, 0xFF}
#define COLOR_ORANGE    {0xFA, 0x65, 0x0F, 0xFF}
#define COLOR_YELLOW    {0xF7, 0xDF, 0x25, 0xFF}
#define COLOR_GREEN     {0x2A, 0xDE, 0x23, 0xFF}
#define COLOR_PURPLE    {0x75, 0x16, 0xFA, 0xFF}

enum Color {
    BLUE = 0,
    RED,
    LIGHT_BLUE,
    ORANGE,
    YELLOW,
    GREEN,
    PURPLE
};

enum FontSize {
    FONTSIZE_DEFAULT,
    FONTSIZE_SMALL,
    FONTSIZE_LARGE
};

typedef struct position_t {
    int x;
    int y;
    position_t() {}
    position_t(int x, int y) : x(x), y(y) {}
    position_t operator+(const position_t& p) {
        return position_t(this->x + p.x, this->y + p.y);
    }
    position_t operator-(const position_t& p) {
        return position_t(this->x - p.x, this->y - p.y);
    }
} position_t;

typedef struct Texture {
    SDL_Texture *image;
    SDL_Rect rect;
} Texture;

#endif