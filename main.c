#include "shadow.h"
#include <raylib.h>
#include <raymath.h>
#include "maps.h"

const int screenWidth = 900;
const int screenHeight = 900;

GAME_STATE state = MENU;
Player player;
Guard guard1;
Guard guard2;
const Map level1 = {.tiles = {
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
              }};

#define DEBUG                                                                  \
  DrawCircleV(guard1.waypoints[0], 10, SKYBLUE);                               \
  DrawCircleV(guard1.waypoints[1], 10, SKYBLUE);                               \
  DrawCircleV(guard2.waypoints[0], 10, MAROON);                                \
  DrawCircleV(guard2.waypoints[1], 10, MAROON);                                \
  DrawGuardDebug(&guard1, guard1.waypoints[1 - guard1.currWaypoint]);          \
  DrawGuardDebug(&guard2, guard2.waypoints[1 - guard2.currWaypoint]);          \
  DrawFps(10, 10);

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

  UpdateGuard(&guard1, 1 - guard1.currWaypoint, delta);
  UpdateGuard(&guard2, 1 - guard2.currWaypoint, delta);

  BeginDrawing();
  {
    ClearBackground(BG);
    DrawMap(&level1);

    DrawGuard(&guard1);
    DrawGuard(&guard2);
    DrawPlayer(&player);

    if (CheckCollisionPlayerGuard(&player, &guard1) ||
        CheckCollisionPlayerGuard(&player, &guard2)) {
      state = GAME_OVER;
      DrawGameOver();
    }

    // DEBUG;
  }
  EndDrawing();
}

int main() {
  InitWindow(screenWidth, screenHeight, "***Shadow Protocol***");
  SetTargetFPS(FPS);

  player = (Player){
      .pos = {center.x - PLAYER_RADIUS / 2, center.y - PLAYER_RADIUS / 2},
      .vel = {0},
      .size = {PLAYER_RADIUS, PLAYER_RADIUS},
      .hasChip = false,
  };

  guard1 = (Guard){
      .facing = PI / 2,
      .currWaypoint = 0,
      .waypoints = {{center.x - 200, center.y - 100},
                    {center.x - 200, center.y + 100}},
      .walking = false,
  };
  guard1.pos = guard1.waypoints[0];

  guard2 = (Guard){
      .facing = PI / 2,
      .currWaypoint = 0,
      .waypoints = {{center.x + 100, center.y - 100},
                    {center.x + 100, center.y + 100}},
      .walking = false,
  };
  guard2.pos = guard2.waypoints[0];

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
      if (IsKeyPressed(KEY_SPACE))
        state = GAME;
    }
  }

  CloseWindow();
  return 0;
}
