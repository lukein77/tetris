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

void Renderer::clearScene() {
    // clear everything
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
}

void Renderer::renderScene() {
    // render everything
    SDL_RenderPresent(renderer);
}

void Renderer::drawSquare(int x, int y, SDL_Color color, bool fill) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_Rect rect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE-1, BLOCK_SIZE-1};
    
    if (fill) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Renderer::drawBlock(int x, int y, Color color)
{
    SDL_Rect rect = {x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderCopy(renderer, block_textures[color], NULL, &rect);
}

void Renderer::drawGrid() {
    // grid
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            drawSquare(i, j, {0x26, 0x26, 0x26, 0xFF});
        }
    }
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

void Renderer::renderGameOver() {

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x80);
    
    SDL_Rect r;
    r.w = GRID_WIDTH * BLOCK_SIZE;
    r.h = GRID_HEIGHT * BLOCK_SIZE;
    r.x = 0;
    r.y = 0;
    
    SDL_RenderFillRect(renderer, &r);

    renderText("GAME OVER", GRID_WIDTH / 2 * BLOCK_SIZE, GRID_HEIGHT / 2 * BLOCK_SIZE, FONTSIZE_DEFAULT, COLOR_WHITE, true);
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