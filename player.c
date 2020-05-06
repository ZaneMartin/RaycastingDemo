#include <math.h>
#include <SDL2/SDL.h>

#include "player.h"
#include "map.h"

void Player_move(struct Player* player, struct Map* map)
{
	// get the keyboard state fron SDL
	const unsigned char* keystates = SDL_GetKeyboardState(NULL);
	// If one of the movement keys is pressed
	if (keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_D]) {
		// work out what direction the player should move based on which keys are pressed anf the current angle
		double dir = atan2(keystates[SDL_SCANCODE_D] - keystates[SDL_SCANCODE_A], keystates[SDL_SCANCODE_W] - keystates[SDL_SCANCODE_S]) + player->angle;

		// calculate the distinct orthoganal velocities
		double xVelo = player->movespeed * cos(dir);
		double yVelo = player->movespeed * sin(dir);

		// move along each axis, checking for collisions
		if (!map->tiles[map->width * (int) player->y + (int) (player->x + xVelo)]) {
			player->x += xVelo;
		}
		if (!map->tiles[map->width * (int) (player->y + yVelo) + (int) player->x]) { 
			player->y += yVelo;
		}
	}

	// Update the player's angle
	player->angle += player->turnspeed * (keystates[SDL_SCANCODE_RIGHT] - keystates[SDL_SCANCODE_LEFT]);

	// Correct angle to be in the range [0, 2PI)
	if (player->angle < 0) { 
		player->angle += 2 * M_PI;
	} else if (player->angle >= 2 * M_PI) { 
		player->angle -= 2 * M_PI;
	}
}