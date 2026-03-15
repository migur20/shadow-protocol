#include "maps.h"
#include <asm-generic/errno.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

const int screenWidth = 880;
const int screenHeight = 880;

GAME_STATE state = MENU;
Player player;

Map *level1;

const int level1Tiles[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, CHIP, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, SPAWN, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

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
  DrawText("SHADOW", 50, 50, 100, WHITE);
  DrawText("PROTOCOL", 50, 150, 100, WHITE);
}

void GameLoop() {
  float delta = GetFrameTime();
  HandlePlayerMovement(&player, delta);
  MapCollisions(&player, &level1->walls);

  Rectangle pRect = {.x = player.pos.x,
                     .y = player.pos.y,
                     .width = player.size.x,
                     .height = player.size.y};
  if (CheckCollisionCircleRec(level1->chipPos, CHIP_RADIUS, pRect)) {
    player.hasChip = true;
  }

  BeginDrawing();
  {
    ClearBackground(BG);

    for (int i = 0; i < level1->guards.lenght; i++) {
      UpdateGuard(&level1->guards.guards[i],
                  1 - level1->guards.guards[i].currWaypoint, delta);
      DrawGuard(&level1->guards.guards[i]);
    }

    DrawMap(level1Tiles, player.hasChip);

    DrawPlayer(&player);

    // TODO
    // Collisions with guards
    if (0) {
      // state = GAME_OVER;
      printf("GAMEOVER\n");
      DrawGameOver();
    }
    DEBUG;
  }
  EndDrawing();
}

int main() {
  InitWindow(screenWidth, screenHeight, "***Shadow Protocol***");
  SetTargetFPS(FPS);

  level1 = LoadMap(level1Tiles);

  player = (Player){
      .pos = level1->spawn,
      .vel = {0},
      .size = {PLAYER_RADIUS, PLAYER_RADIUS},
      .hasChip = false,
  };

  while (!WindowShouldClose()) {
    if (state == MENU) {
      BeginDrawing();
      DrawMenu();
      DrawText(RESTART_TEXT, center.x, center.y, RESTART_TEXT_SIZE, WHITE);
      EndDrawing();
      if (IsKeyPressed(KEY_SPACE))
        state = GAME;
    } else if (state == GAME) {
      GameLoop();
    } else if (state == GAME_OVER) {
      if (IsKeyPressed(KEY_SPACE))
        state = GAME;
    }
  }

  CloseWindow();
  return 0;
}
