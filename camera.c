#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#include "camera.h"
#include "player.h"
#include "map.h"

static void drawVerticalLine(struct SDL_Renderer* renderer, double distance, int x, int Screen_Height);

void raycast(struct SDL_Renderer* renderer, struct SDL_Texture* wall, struct Map* map, struct Player* player, int Screen_Width, int Screen_Height)
{
	int wallWidth, wallHeight;
	// Get the dimensions of the wall texture so we know how to render it
	SDL_QueryTexture(wall, NULL, NULL, &wallWidth, &wallHeight);
	

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
	// Need a pi constant - std library trig functions expect radian input
	// the camera needs a field of view. Play with this until it looks good? 
	const double FoV = M_PI_2;

	// For calculating the angle; The amount the angle changes isnt fixed,
	// this calculates so the only thing we need to change to adjust FoV
	// is the above FoV variable.
	const double distToCamera = 0.5f;
	const double cameraWidth = 2 * distToCamera * tan(FoV / 2);
	const double widthStep = cameraWidth / Screen_Width;

	// player angle is exactly halfway through the screen, so since we scan left-to-right, set current angle 
	// to the far left
	// Angles increase clockwise from +x => +y => -x => -y => +x
	double currentAngle = (player->angle - (FoV / 2));

	for (int i = -Screen_Width / 2; i < Screen_Width / 2; ++i) {
		// For each ray, start checking starting at player location
		double x = player->x;
		double y = player->y;

		// y-axis distance to next whole x-val
		double deltaX;
		// x-axis distance to next whole y-val
		double deltaY;

		// linear distances to respective next whole x- and y-vals
		double distToX;
		double distToY;

		// calculate current angle
		 currentAngle = player->angle + atan( (i * widthStep) / distToCamera );

		// Adjust angle range
		// Technically there's a bug here if currentAngle < -2PI or currentAngle >= 4PI
		// But that shouldn't happen. If it does, this will make it obvious there's a bug elsewhere.
		if (currentAngle < 0) {
			currentAngle += 2 * M_PI;
		} else if (currentAngle >= 2 * M_PI) {
			currentAngle -= 2 * M_PI;
		}

		// Calculate the x-distance to the next integer x-val
		// If x is currently an integer, then it is a full step to the next int,
		// otherwise,
		while ((x > 1) && (x < map->width) && (y > 1) && (y < map->width)) {
			if ((currentAngle < M_PI_2) || (currentAngle > 3 * M_PI_2)) {
			// Increasing x
				deltaX = (x == ceil(x)) ? 1 : ceil(x) - x;
			} else {
				// Decreasing x
				deltaX = (x == floor(x)) ? -1 : floor(x) - x;
			}

			// do the same for the next integer y-val;
			if (currentAngle < M_PI) {
				deltaY = (y == ceil(y)) ? 1 : ceil(y) - y;
			} else {
				deltaY = (y == floor(y)) ? -1 : floor(y) - y;
			}

			// Calculate absolute distance using deltaX = h*cos(theta), deltaY = h*sin(theta)
			// Two special cases: if axis-aligned then the distance to the next int value for 
			// one of the two is infinity
			if (sin(currentAngle) == 0.0f) {
				distToX = deltaX;
				distToY = INFINITY;
			} else if (sin(currentAngle) == 1.0f) {
				distToX = INFINITY;
				distToY = deltaY;
			} else {
				// We only care about absolute distance
				distToX = fabs(deltaX / cos(currentAngle));
				distToY = fabs(deltaY / sin(currentAngle));
			}

			// calculate distance to current point from camera plane
			if (distToX < distToY) {
				// calculate new position, x + deltax, y + deltax * tan(theta)
				// Then round the whole number value to ensure we avoid floating point errors

				x = round(x + deltaX);
				y += deltaX * tan(currentAngle);

				// calculate distance to wall from the player, and adjust it to be distance 
				// to the plane perpendicular to the player's angle
				double distance = cos(fabs(player->angle - currentAngle)) * sqrt( pow(x - player->x, 2) + pow(y - player->y, 2) );

				// Which side we hit can change based on what the angle is
				if ((currentAngle < M_PI_2) || (currentAngle > 3 * M_PI_2)) {
					// increasing x means we hit the right side and need to check at (x, floor(y))
					if (map->tiles[map->width * (int) floor(y) + (int) x]) {
						// calculate which vertical line of the wall texture to draw
						struct SDL_Rect srcRect = {
							(int) ((y - floor(y)) * wallWidth ),
							0,
							1, 
							wallHeight };
						// Caculate where on the screen to draw it
						struct SDL_Rect destRect = {
							i + (Screen_Width / 2), 
							(int) ((Screen_Height / 2) * (1 - 1 / distance)),
							1, 
							(int) (Screen_Height / distance) };
						// Draw
						SDL_RenderCopy(renderer, wall, &srcRect, &destRect);
		break; 
					}
				} else {
					// decreasing x means we hit the left side and need to check (x - 1, floor(y))
					if (map->tiles[map->width * (int) floor(y) + (int) x - 1]){
						// calculate which vertical line of the wall texture to draw
						struct SDL_Rect srcRect = {
							(int) ((y - floor(y)) * wallWidth ),
							0,
							1, 
							wallHeight };
						// Caculate where on the screen to draw it
						struct SDL_Rect destRect = {
							i + (Screen_Width / 2), 
							(int) ((Screen_Height / 2) * (1 - 1 / distance)),
							1, 
							(int) (Screen_Height / distance) };
						// Draw
						SDL_RenderCopy(renderer, wall, &srcRect, &destRect);
		break;
					}
				}
			} else {
				x += deltaY * (1 / tan(currentAngle));
				y = round(y + deltaY);

				double distance = cos(fabs(player->angle - currentAngle)) * sqrt( pow(x - player->x, 2) + pow(y - player->y, 2) );

				if (currentAngle < M_PI) {
					// Increasing y means we hit the top and need to check (floor(x), y)
					if (map->tiles[map->width * (int) y + (int) floor(x)]) {
						// calculate which vertical line of the wall texture to draw
						struct SDL_Rect srcRect = {
							(int) ((x - floor(x)) * wallWidth ),
							0,
							1, 
							wallHeight };
						// Caculate where on the screen to draw it
						struct SDL_Rect destRect = {
							i + (Screen_Width / 2), 
							(int) ((Screen_Height / 2) * (1 - 1 / distance)),
							1, 
							(int) (Screen_Height / distance) };
						// Draw
						SDL_RenderCopy(renderer, wall, &srcRect, &destRect);
		break;
					}
				} else {
					// Decreaing y means we hit the bottom and need to check (floor(x), y - 1)
					if (map->tiles[map->width * ((int) y - 1) + (int) floor(x)]) {
						// calculate which vertical line of the wall texture to draw
						struct SDL_Rect srcRect = {
							(int) ((x - floor(x)) * wallWidth ),
							0,
							1, 
							wallHeight };
						// Caculate where on the screen to draw it
						struct SDL_Rect destRect = {
							i + (Screen_Width / 2), 
							(int) ((Screen_Height / 2) * (1 - 1 / distance)),
							1, 
							(int) (Screen_Height / distance) };
						// Draw
						SDL_RenderCopy(renderer, wall, &srcRect, &destRect);
		break;
					}
				}
			}
		}
	}
}

static void drawVerticalLine(struct SDL_Renderer* renderer, double distance, int x, int Screen_Height)
{
	SDL_RenderDrawLine(renderer, x, (Screen_Height / 2) * (1 - 1 / distance), x, (Screen_Height / 2) * (1 + 1 / distance) );
}