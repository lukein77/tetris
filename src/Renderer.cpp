#include "Renderer.h"
#include <SDL2/SDL_image.h>

Renderer::Renderer() {}

Renderer::~Renderer() {
    for (int i = 0; i < 3; i++) {
        TTF_CloseFont(fonts[i]);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
}


/// @brief Initializes SDL library, creates the window, loads fonts and block textures
/// @return True if initialization was successfull, false if not
bool Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("TETRIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Failed to open window: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    
    if (TTF_Init() == -1) { 
		printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
		return false;
	} else {
		fonts[FONTSIZE_DEFAULT] = TTF_OpenFont("graphics/PressStart2P.ttf", 18);
        fonts[FONTSIZE_SMALL] = TTF_OpenFont("graphics/PressStart2P.ttf", 14);
        fonts[FONTSIZE_LARGE] = TTF_OpenFont("graphics/PressStart2P.ttf", 36);
	}

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }

    for (int i = 0; i < 7; i++) {
        block_textures[i] = loadTexture("graphics/block_"+std::to_string(i)+".png");
        if (!block_textures[i]) return false;
    }
    
    return true;
}

/// @brief Clears the scene by drawing a black background on top of everything
void Renderer::clearScene() {
    // clear everything
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
}

/// @brief Update the screen and draw all elements
void Renderer::renderScene() {
    // render everything
    SDL_RenderPresent(renderer);
}

/// @brief Draw a square of size BLOCK_SIZE.
/// @param x position on the x axis
/// @param y position on the y axis
/// @param color the square's color
/// @param fill filled square or not
void Renderer::drawSquare(int x, int y, SDL_Color color, bool fill) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_Rect rect = {x * 40, y * 40, 40, 40};
    
    if (fill) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

/// @brief Draw a Tetris block on the corresponding position
/// @param x position on the x axis
/// @param y position on the y axis
/// @param color block color
void Renderer::drawBlock(float x, float y, Color color)
{
    SDL_Rect rect = {(int) x * (BLOCK_SIZE-3), (int) y * (BLOCK_SIZE-3), BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderCopy(renderer, block_textures[color], NULL, &rect);
}

/// @brief Draw a background for rendering the Figures on top of it
void Renderer::drawGrid() {
    /*
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            drawSquare(i, j, {0x26, 0x26, 0x26, 0xFF});
        }
    }*/
    SDL_SetRenderDrawColor(renderer, 0x25, 0x25, 0x25, 0xFF);
    SDL_Rect rect = {0, 0, GRID_WIDTH*(BLOCK_SIZE-3), GRID_HEIGHT*(BLOCK_SIZE)};
    SDL_RenderFillRect(renderer, &rect);
    drawSquare(50, 50, COLOR_RED, true);

}

void Renderer::blit(Texture *texture, int x, int y) {
	
	texture->rect.x = x;
	texture->rect.y = y;
	
	SDL_RenderCopy(renderer, texture->image, NULL, &(texture->rect));
}

void Renderer::renderText(std::string text, int x, int y, int size, SDL_Color color, bool centered) {

	SDL_Surface *surface = TTF_RenderText_Solid(fonts[size], text.c_str(), color);

	if (surface) {
		Texture texture;
		texture.image = SDL_CreateTextureFromSurface(renderer, surface);
		texture.rect.w = surface->w;
		texture.rect.h = surface->h;

		if (centered) {
			x = x - surface->w / 2;
			y = y - surface->h / 2;
		}

		blit(&texture, x, y);
	} else {
		printf("Error rendering text: %s\n", TTF_GetError());
	}
}

/// @brief Display a message over the grid. Used for "GAME OVER" or "PAUSE" messages.
/// @param text Text to display
void Renderer::renderMessage(std::string text) {

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x80);
    
    SDL_Rect r;
    r.w = GRID_WIDTH * BLOCK_SIZE;
    r.h = GRID_HEIGHT * BLOCK_SIZE;
    r.x = 0;
    r.y = 0;
    
    SDL_RenderFillRect(renderer, &r);

    renderText(text, GRID_WIDTH / 2 * BLOCK_SIZE, GRID_HEIGHT / 2 * BLOCK_SIZE, FONTSIZE_DEFAULT, COLOR_WHITE, true);
}

SDL_Texture *Renderer::loadTexture(std::string filename)
{
    SDL_Texture *texture = NULL;

    SDL_Surface *surface = IMG_Load(filename.c_str());
    
    if (surface == NULL) {
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        return nullptr;
    }

    SDL_FreeSurface(surface);

    return texture;
}