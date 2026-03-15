#include <raylib.h>

#define MAPS_DEFIN

#define WALL_COLOR LIGHTGRAY
#define WALL_THICKNESS 4

#define MAP_WIDTH 22
#define MAP_HEIGHT 22
#define TILE_SIZE 40

typedef enum {
    EMPTY = 0,
    WALL = 1,
    CHIP = 2,
    SPAWN = 3
} TileType;

typedef struct {
    int tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;

void DrawMap(Map *m);

