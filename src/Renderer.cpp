#include "Renderer.h"

Renderer::Renderer() {}

Renderer::~Renderer() {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

bool Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("EL TRETIRS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
		fonts[FONTSIZE_DEFAULT] = TTF_OpenFont("data/PressStart2P.ttf", 15);
        fonts[FONTSIZE_SMALL] = TTF_OpenFont("data/PressStart2P.ttf", 12);
        fonts[FONTSIZE_LARGE] = TTF_OpenFont("data/PressStart2P.ttf", 18);
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
    
    SDL_Rect r;
    r.w = BLOCK_SIZE;
    r.h = BLOCK_SIZE;
    r.x = x * BLOCK_SIZE;
    r.y = y * BLOCK_SIZE;
    
    if (fill) {
        SDL_RenderFillRect(renderer, &r);
    } else {
        SDL_RenderDrawRect(renderer, &r);
    }
}

void Renderer::blit(Texture *texture, int x, int y) {
	
	texture->rect.x = x;
	texture->rect.y = y;
	
	SDL_RenderCopy(renderer, texture->image, NULL, &(texture->rect));
}

void Renderer::renderText(const char *text, int x, int y, int size, SDL_Color color, bool centered) {

	SDL_Surface *surface = TTF_RenderText_Solid(fonts[size], text, color);

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