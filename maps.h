#include <raylib.h>
#include "shadow.h"

#define MAPS_DEFIN

#define WALL_COLOR (Color){0x4A,0x48,0x45,0xFF}
#define WALL_THICKNESS 4

#define MAP_WIDTH 22
#define MAP_HEIGHT 22
#define TILE_SIZE 40

#define CHIP_COLOR SKYBLUE

typedef enum {
    EMPTY = 0,
    WALL = 1,
    CHIP = 2,
    SPAWN = 3,
} TileType;

typedef struct{
	Rectangle *walls;
	int lenght;
	int size;
}Walls;

typedef struct{
	Guard *guards;
	int lenght;
	int size;
}Guards;

typedef struct {
	Walls walls;
	Guards guards;
	Vector2 chipPos;
	Vector2 spawn;
} Map;


void DrawMap(const int tiles[MAP_HEIGHT][MAP_WIDTH], bool hasChip);

Map *LoadMap(const int tiles[MAP_HEIGHT][MAP_WIDTH]);

void MapCollisions(Player *p, Walls *walls);
