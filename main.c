#include "shadow.h"
#include <raylib.h>
#include <stdio.h>
#include <raymath.h>

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
#define center (Vector2){(double)screenWidth / 2, (double)screenHeight / 2}

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
      .facing = PI/2,
      .currWaypoint = 0,
			.waypoints = {{center.x - 100, center.y - 100}, {center.x + 100, center.y + 100}},
			.walking = false,
  };
	guard1.pos = guard1.waypoints[0];

  Guard guard2 = {
      .facing = PI/2,
			.currWaypoint = 0,
			.waypoints = {{center.x - 100, center.y + 100}, {center.x + 100, center.y - 100}},
			.walking = false,
	};
	guard2.pos = guard2.waypoints[0];

	Vector2 move;
	double delta;
	while (!WindowShouldClose()) {
		delta = GetFrameTime();
		if(delta == 0)
			delta = 1.0 / FPS;

		//Player Movement
		HandlePlayerMovement(&player, delta);

		UpdateGuard(&guard1, 1 - guard1.currWaypoint, delta);
		UpdateGuard(&guard2, 1 - guard2.currWaypoint, delta);

    BeginDrawing();
    {
      ClearBackground(BG);

      DrawGuard(&guard1);
      DrawGuard(&guard2);
      DrawPlayer(&player);

			DrawCircleV(guard1.waypoints[0], 10, SKYBLUE);
			DrawCircleV(guard1.waypoints[1], 10, SKYBLUE);
			DrawCircleV(guard2.waypoints[0], 10, MAROON);
			DrawCircleV(guard2.waypoints[1], 10, MAROON);
    }
    DrawFPS(5, 5);
    EndDrawing();

		// int c;
		// if((c = getchar()) == 'n')
		// 	continue;
		// else if(c == 'q')
		// 	break;
  }

  CloseWindow();
  return 0;
}
