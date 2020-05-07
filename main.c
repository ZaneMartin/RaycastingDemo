/// Standard Library Headers
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>

/// SDL Headers
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/// Local Headers
#include "camera.h"
#include "player.h"
#include "map.h"

int main(int argc, char* argv[])
{
	// Screen size constants
	const int Screen_Width = 1280;
	const int Screen_Height = 720;

	// Tracks what to cleanup in case of an error during initialization
	int errorFlag = 0;

	// SDL structures for the window and renderer.
	struct SDL_Window* mainWindow = NULL;
	struct SDL_Renderer* primaryRenderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		errorFlag = 1;
		goto ERROR;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		fprintf(stderr, "Error initializing SDL_image subsystem: %s\n", SDL_GetError());
		errorFlag = 2;
		goto ERROR;
	}

	mainWindow = SDL_CreateWindow("My First First Person Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Screen_Width, Screen_Height, SDL_WINDOW_SHOWN);
	if (!mainWindow) {
		fprintf(stderr, "Error creating a window: %s\n", SDL_GetError());
		errorFlag = 3;
		goto ERROR;
	}

	primaryRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!primaryRenderer) {
		fprintf(stderr, "Error creating hardware renderer: %s\n", SDL_GetError());
		errorFlag = 4;
		goto ERROR;
	}

	struct Map* map = loadMap("lvl1.map");
	if (!map) {
		fprintf(stderr, "Error loading level \n");
		errorFlag = 5;
		goto ERROR;
	}

	struct SDL_Texture* wall = IMG_LoadTexture(primaryRenderer, "wall.png");
	if (!wall) { 
		fprintf(stderr, "Error loading wall texture: %s\n", SDL_GetError());
		errorFlag = 6;
		goto ERROR;
	}

	struct Player pc = { 1.5f, 1.5f, 0.05f, M_PI / 2, M_PI / 100};

	for (union SDL_Event event;;) {
		// Pump all events, but only handle Quit events. All interaction will
		// be handled with keystates
		while (SDL_PollEvent(&event)) { 
			if (event.type == SDL_QUIT) { 
				goto breakmainloop;
			}
		}

		// Move player:
		Player_move(&pc, map);

		// Render background
		SDL_SetRenderDrawColor(primaryRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(primaryRenderer);

		// Then raycast walls
		raycast(primaryRenderer, wall, map, &pc, Screen_Width, Screen_Height);

		SDL_RenderPresent(primaryRenderer);
	} breakmainloop: ;

ERROR:
	switch (errorFlag) {
		case 0: SDL_DestroyTexture(wall);
		case 6: freeMap(map);
		case 5: SDL_DestroyRenderer(primaryRenderer);
		case 4: SDL_DestroyWindow(mainWindow);
		case 3: IMG_Quit();
		case 2: SDL_Quit();
		case 1: return errorFlag;
	}
}
