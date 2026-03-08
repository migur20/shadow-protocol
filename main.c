#include "shadow.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define DEATH_TEXT "GAME OVER"
#define DEATH_TEXT_SIZE 50.0
#define DEATH_TEXT_WIDTH MeasureText(DEATH_TEXT, DEATH_TEXT_SIZE)
#define DEATH_TEXT_Y center.y - DEATH_TEXT_SIZE / 2

#define RESTART_TEXT "[ press space to restart ]"
#define RESTART_TEXT_SIZE DEATH_TEXT_SIZE / 2
#define RESTART_TEXT_WIDTH MeasureText(RESTART_TEXT, RESTART_TEXT_SIZE)
#define RESTART_TEXT_Y DEATH_TEXT_Y + DEATH_TEXT_SIZE

const int screenWidth = 900;
const int screenHeight = 900;
const Vector2 center = {(double)screenWidth / 2, (double)screenHeight / 2};

void DrawGameOver() {
  DrawText(DEATH_TEXT, center.x - (double)DEATH_TEXT_WIDTH / 2, DEATH_TEXT_Y,
           DEATH_TEXT_SIZE, YELLOW);
  DrawText(RESTART_TEXT, center.x - (double)RESTART_TEXT_WIDTH / 2,
           RESTART_TEXT_Y, RESTART_TEXT_SIZE, YELLOW);
}

int main() {
  GAME_STATE state = GAME;

  InitWindow(screenWidth, screenHeight, "***Shadow Protocol***");
  SetTargetFPS(FPS);

  Player player = {
      .pos = {center.x - PLAYER_RADIUS / 2, center.y - PLAYER_RADIUS / 2},
      .vel = {0},
      .size = {PLAYER_RADIUS, PLAYER_RADIUS},
  };

  Guard guard1 = {
      .facing = 0,
      .currWaypoint = 0,
			.waypoints = {{center.x - 100, center.y - 100}, {center.x + 100, center.y - 100}},
			.walking = false,
  };
	guard1.pos = guard1.waypoints[0];

  Vector2 move;
  double delta;
  while (!WindowShouldClose()) {
    delta = GetFrameTime();
    move = (Vector2){IsKeyDown(RIGHT_KEY) - IsKeyDown(LEFT_KEY),
                     IsKeyDown(DOWN_KEY) - IsKeyDown(UP_KEY)};
    // Player Movement
    HandlePlayerMovement(&player, move, delta);
    player.pos = Vector2Add(player.pos, player.vel);

		UpdateGuard(&guard1, 1, delta);

    BeginDrawing();
    {
      ClearBackground(BG);
      DrawGuard(&guard1);
      DrawPlayer(&player);

			DrawCircleV(guard1.waypoints[0], 10, SKYBLUE);
			DrawCircleV(guard1.waypoints[1], 10, SKYBLUE);
    }
    DrawFPS(5, 5);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
