#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

struct SDL_Renderer;
struct Map;
struct Player;

// raycast: draw the current view to the screen
// input: 
//	struct SDL_Renderer* - a pointer to the renderer which renders to the screen
//	struct Map*          - a pointer to the map which the player is in
//	struct Player*       - a pointer to the player
//	int Screen_Width     - The width of the screen in pixels
//	int Screen_Height    - The height of the screen in pixels
void raycast(struct SDL_Renderer* renderer, struct SDL_Texture* wall, struct Map* map, struct Player* player, int Screen_Width, int Screen_Height);

#endif