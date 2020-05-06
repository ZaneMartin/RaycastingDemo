#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

struct Map {
    int width;
    int height;
    int* tiles;
};

struct Map* loadMap(const char* path);
void freeMap(struct Map*);

#endif