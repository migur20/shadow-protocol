#include "maps.h"
#include <asm-generic/errno.h>
#include <raylib.h>
#include <raymath.h>
#include "madeMaps.h"

const int screenWidth = 880;
const int screenHeight = 880;

GAME_STATE state = MENU;
Player player;
Map *currLevel;
int currlevelTiles[MAP_HEIGHT][MAP_WIDTH] = LEVEL1;

#define DEBUG DrawFPS(10, 10);

#define DEATH_TEXT "GAME OVER"
#define DEATH_TEXT_SIZE 50.0
#define DEATH_TEXT_WIDTH MeasureText(DEATH_TEXT, DEATH_TEXT_SIZE)
#define DEATH_TEXT_Y center.y - DEATH_TEXT_SIZE / 2

#define RESTART_TEXT "[ press space to restart ]"
#define RESTART_TEXT_SIZE DEATH_TEXT_SIZE / 2
#define RESTART_TEXT_WIDTH MeasureText(RESTART_TEXT, RESTART_TEXT_SIZE)
#define RESTART_TEXT_Y DEATH_TEXT_Y + DEATH_TEXT_SIZE

#define center (Vector2){(double)screenWidth / 2, (double)screenHeight / 2}

void DrawGameOver() {
  DrawText(DEATH_TEXT, center.x - (double)DEATH_TEXT_WIDTH / 2, DEATH_TEXT_Y,
           DEATH_TEXT_SIZE, YELLOW);
  DrawText(RESTART_TEXT, center.x - (double)RESTART_TEXT_WIDTH / 2,
           RESTART_TEXT_Y, RESTART_TEXT_SIZE, YELLOW);
}

void DrawMenu() {
  DrawText("SHADOW", 70, 50, 100, WHITE);
  DrawText("PROTOCOL", 70, 150, 100, WHITE);
}

void GameLoop() {
  float delta = GetFrameTime();
  HandlePlayerMovement(&player, delta);
  MapCollisions(&player, &currLevel->walls);

  Rectangle pRect = {.x = player.pos.x,
                     .y = player.pos.y,
                     .width = player.size.x,
                     .height = player.size.y};
  if (CheckCollisionCircleRec(currLevel->chipPos, CHIP_RADIUS, pRect)) {
    player.hasChip = true;
  }

  BeginDrawing();
  {
    ClearBackground(BG);

    for (int i = 0; i < currLevel->guards.lenght; i++) {
      UpdateGuard(&currLevel->guards.guards[i],
                  1 - currLevel->guards.guards[i].currWaypoint, delta);
      DrawGuard(&currLevel->guards.guards[i]);
    }

    DrawMap(currlevelTiles, player.hasChip);

    DrawPlayer(&player);

    // Collisions with guards
    for (int i = 0; i < currLevel->guards.lenght; i++) {
      if (CheckCollisionPlayerGuard(&player, &currLevel->guards.guards[i])) {
        state = GAME_OVER;
      }
    }
    // DEBUG;
  }
  EndDrawing();
}

int main() {
  InitWindow(screenWidth, screenHeight, "***Shadow Protocol***");
  SetTargetFPS(FPS);

  Map *level1 = LoadMap(currlevelTiles);
  currLevel = level1;

  player = (Player){
      .pos = currLevel->spawn,
      .vel = {0},
      .size = {PLAYER_RADIUS, PLAYER_RADIUS},
      .hasChip = false,
  };

  while (!WindowShouldClose()) {
    if (state == MENU) {
      BeginDrawing();
      DrawMenu();
      EndDrawing();
      if (IsKeyPressed(KEY_SPACE))
        state = GAME;
    } else if (state == GAME) {
      GameLoop();
    } else if (state == GAME_OVER) {
      if (IsKeyPressed(KEY_SPACE)) {
        state = GAME;
        player.pos = currLevel->spawn;
        player.vel = (Vector2){0};
        player.hasChip = false;
        for (int i = 0; i < level1->guards.lenght; i++) {
          currLevel->guards.guards[i].pos =
              currLevel->guards.guards[i].waypoints[0];
          currLevel->guards.guards[i].facing = 0;
        }
      }
      BeginDrawing();
      DrawGameOver();
      EndDrawing();
    }
  }

  CloseWindow();
  return 0;
}
