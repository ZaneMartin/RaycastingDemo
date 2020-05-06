#include <stdio.h>
#include <stdlib.h>
#include "map.h"

struct Map* loadMap(const char* path)
{
    FILE* level = fopen(path, "r");
    if (!level) {
        fprintf(stderr, "Error opening map \"%s\"\n", path);
        return NULL;
    }

    int width, height;
    fscanf(level, "%d %d", &width, &height);

    struct Map* loadedLevel = malloc(sizeof(struct Map));
    loadedLevel->height = height;
    loadedLevel->width = width;
    loadedLevel->tiles = calloc(width * height, sizeof(int));

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