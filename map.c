#include <stdio.h>
#include <stdlib.h>
#include "map.h"

struct Map* loadMap(const char* path)
{
	// open path for reading
	FILE* level = fopen(path, "r");
	if (!level) {
		fprintf(stderr, "Error opening map \"%s\"\n", path);
		return NULL;
	}

	// first two values are the width and height of the map
	int width, height;
	fscanf(level, "%d %d", &width, &height);

	// allocate enough memory for the map structure and tiles
	struct Map* loadedLevel = malloc(sizeof(struct Map));
	if (!loadedLevel) {
		fprintf(stderr, "malloc error\n");
		fclose(level);
		return NULL;
	}

	loadedLevel->height = height;
	loadedLevel->width = width;
	loadedLevel->tiles = calloc(width * height, sizeof(int));
	
	if (!loadedLevel->tiles) {
		fprintf(stderr, "malloc error \n");
		free(loadedLevel);
		fclose(level);
		return NULL;
	}


	for (int y = 0; y < height; y++) { 
		for (int x = 0; x < width; x++) { 
			fscanf(level, "%d", loadedLevel->tiles + (width * y) + x);
		}
	}

	fclose(level);

	return loadedLevel;
}

void freeMap(struct Map* map) 
{
	free(map->tiles);
	free(map);
}