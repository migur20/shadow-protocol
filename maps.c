#include "maps.h"
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>

void DrawWall(Rectangle tileRect) {
  DrawRectangleRec(tileRect, WALL_COLOR);
  DrawRectangleLinesEx(tileRect, 1, BLACK);
}

void DrawMap(const int tiles[MAP_HEIGHT][MAP_WIDTH], bool hasChip) {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      Rectangle tileRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

      if (tiles[y][x] == WALL) {
        DrawWall(tileRect);
      } else if (!hasChip && tiles[y][x] == CHIP) {
        DrawCircle(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2,
                   CHIP_RADIUS, CHIP_COLOR);
      }
    }
  }
}

Vector2 GetPlayerTile(Player *p) {
  return (Vector2){(int)p->pos.x % TILE_SIZE, (int)p->pos.y % TILE_SIZE};
}

bool CheckEmptyTiles(const int tiles[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
  if (y != 0 && tiles[y - 1][x] == EMPTY)
    return true;
  if (x != 0 && tiles[y][x - 1] == EMPTY)
    return true;
  if (x < MAP_WIDTH - 1 && tiles[y][x + 1] == EMPTY)
    return true;
  if (y < MAP_HEIGHT - 1 && tiles[y + 1][x] == EMPTY)
    return true;
  return false;
}

#define WALL_BUF_SIZE 512
#define GUARD_BUF_SIZE 5

void MapAddWall(Map *map, int x, int y) {
  if (map->walls.lenght >= map->walls.size) {
    map->walls.size += WALL_BUF_SIZE / 2;
    map->walls.walls =
        realloc(map->walls.walls, sizeof(Rectangle) * map->walls.size);
  }
  map->walls.walls[map->walls.lenght++] = (Rectangle){.x = x * TILE_SIZE,
                                                      .y = y * TILE_SIZE,
                                                      .width = TILE_SIZE,
                                                      .height = TILE_SIZE};
}

Map *LoadMap(const int tiles[MAP_HEIGHT][MAP_WIDTH]) {
  Map *map = malloc(sizeof(*map));

  map->walls.lenght = 0;
  map->walls.size = WALL_BUF_SIZE;
  map->walls.walls = calloc(map->walls.size, sizeof(Rectangle));

  map->guards.lenght = 0;
  map->guards.size = GUARD_BUF_SIZE;
  map->guards.guards = calloc(map->guards.size, sizeof(Guard));

  int guardWaypoints[GUARD_BUF_SIZE] = {0};
  int nGW = 0;

  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      int tile = tiles[y][x];
      if (tile == CHIP) {
        map->chipPos = (Vector2){x * TILE_SIZE + (float)TILE_SIZE / 2,
                                 y * TILE_SIZE + (float)TILE_SIZE / 2};
      } else if (tile == WALL && CheckEmptyTiles(tiles, x, y)) {
        MapAddWall(map, x, y);
      } else if (tile == SPAWN) {
        map->spawn = (Vector2){x * TILE_SIZE + (float)TILE_SIZE / 2,
                               y * TILE_SIZE + (float)TILE_SIZE / 2};
      } else if (tile > SPAWN) {
        bool found = false;
        for (int i = 0; i < nGW; i++) {
          if (guardWaypoints[i] == tile) {
            map->guards.guards[i].waypoints[1] =
                (Vector2){(x * TILE_SIZE) + (float)TILE_SIZE / 2,
                          (y * TILE_SIZE) + (float)TILE_SIZE / 2};
            found = true;
            break;
          }
        }
        if (!found) {
          map->guards.guards[map->guards.lenght++] = (Guard){
              .pos = {(x * TILE_SIZE) + (float)TILE_SIZE / 2,
                      (y * TILE_SIZE) + (float)TILE_SIZE / 2},
              .walking = false,
              .facing = 0,
              .currWaypoint = 0,
              .waypoints = {{(x * TILE_SIZE) + (float)TILE_SIZE / 2,
                             (y * TILE_SIZE) + (float)TILE_SIZE / 2},
                            {0}},
          };
          guardWaypoints[nGW++] = tile;
        }
      }
    }
  }
  return map;
}

void MapCollisions(Player *p, Walls *walls) {
  Rectangle pRect = {
      .x = p->pos.x, .y = p->pos.y, .width = p->size.x, .height = p->size.y};
  int nColls = 0;
  for (int i = 0; i < walls->lenght; i++) {
    Rectangle wall = walls->walls[i];
    if (Vector2Distance((Vector2){wall.x, wall.y}, p->pos) < TILE_SIZE * 2) {
      if (CheckCollisionRecs(pRect, wall)) {
        Rectangle result = GetCollisionRec(pRect, wall);
        if (result.width < result.height) {
          if (pRect.x < wall.x)
            p->pos.x -= result.width;
          else
            p->pos.x += result.width;
          p->vel.x = 0;

        } else {
          if (pRect.y < wall.y)
            p->pos.y -= result.height;
          else
            p->pos.y += result.height;

          p->vel.y = 0;
        }

        nColls++;
        if (nColls >= 3)
          return;
      }
    }
  }
}
