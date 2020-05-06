#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

struct SDL_Renderer;
struct Map;
struct Player;

void raycast(struct SDL_Renderer* renderer, struct Map* map, struct Player* player, int Screen_Width, int Screen_Height);

#endif