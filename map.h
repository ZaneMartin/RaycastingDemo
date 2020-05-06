#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

// struct Map:
// containes:
//	int width
//	int height
//	int* tiles - array of size (width x height)
struct Map {
	int width;
	int height;
	int* tiles;
};

// loadMap:
// takes:
//	const char* - a c-string with the location of the file containing the map
// returns:
//	a pointer to the newly created map structure
// Caller must call freeMap
struct Map* loadMap(const char* path);

// freeMap:
// takes:
//	strut Map* - a pointer to a map structure
// effect:
//	Frees the memory allocated with loadMap
void freeMap(struct Map*);

#endif