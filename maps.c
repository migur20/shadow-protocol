#include "maps.h"
#include <raylib.h>
#include <raymath.h>
#include "shadow.h"

void DrawWall(Rectangle tileRect) {
  DrawRectangleRec(tileRect, DARKGRAY);
  DrawRectangleLinesEx(tileRect, 1, BLACK);
}

void DrawMap(Map *m) {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      Rectangle tileRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

      if (m->tiles[y][x] == WALL) {
				DrawWall(tileRect);
      } else if (m->tiles[y][x] == CHIP) {
        DrawCircle(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2,
                   10, GOLD);
      }
    }
  }
}

Vector2 GetPlayerTile(Player *p){
	return (Vector2){(int)p->pos.x%TILE_SIZE, (int)p->pos.y%TILE_SIZE};
}
