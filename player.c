#include <math.h>
#include <SDL2/SDL.h>

#include "player.h"
#include "map.h"

void Player_move(struct Player* player, struct Map* map)
{
    const unsigned char* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_D]) {
        double dir = atan2(keystates[SDL_SCANCODE_D] - keystates[SDL_SCANCODE_A], keystates[SDL_SCANCODE_W] - keystates[SDL_SCANCODE_S]) + player->angle;
        
        double xVelo = player->movespeed * cos(dir);
        double yVelo = player->movespeed * sin(dir);

        if (!map->tiles[map->width * (int) player->y + (int) (player->x + xVelo)]) {
            player->x += xVelo;
        }
        if (!map->tiles[map->width * (int) (player->y + yVelo) + (int) player->x]) { 
            player->y += yVelo;
        }
    }

    player->angle += player->turnspeed * (keystates[SDL_SCANCODE_RIGHT] - keystates[SDL_SCANCODE_LEFT]);

    if (player->angle < 0) { 
        player->angle += 2 * M_PI;
    } else if (player->angle >= 2 * M_PI) { 
        player->angle -= 2 * M_PI;
    }
}