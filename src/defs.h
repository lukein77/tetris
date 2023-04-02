#ifndef _DEFS_H_DEFINED_
#define _DEFS_H_DEFINED_

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define BLOCK_SIZE 30

#define COLOR_RED       {0x8B, 0x00, 0x00, 0xFF}
#define COLOR_BLUE      {0x00, 0x00, 0x8B, 0xFF}

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
} position_t;

typedef struct Texture {
    SDL_Texture *image;
    SDL_Rect rect;
} Texture;

#endif