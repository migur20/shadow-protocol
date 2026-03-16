#include "shadow.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

char logBuf[128];

void Log(Player player) {
  sprintf(logBuf, "pos(%.2f,%.2f) vel(%.2f,%.2f)", player.pos.x, player.pos.y,
          player.vel.x, player.vel.y);
  DrawText(logBuf, 10, 10, 40, BLACK);
}

void DrawPlayer(Player *p) {
  DrawRectangleV(p->pos, p->size, PLAYER_COLOR);
  if (p->hasChip)
    DrawCircle(p->pos.x + p->size.x / 2, p->pos.y + p->size.y / 2,
               p->size.x / 4, CHIP_COLOR);
}

Vector2 GetGuardFacing(Guard *g) {
  // if (g->facing > 0)
  //   return Vector2Rotate(FACING_ZERO_VEC, 2*PI - g->facing);
  // else
  return Vector2Rotate(FACING_ZERO_VEC, g->facing);
}

Vector2 *GetVertices(Guard *g) {
  Vector2 *vertices = malloc(sizeof(Vector2) * 3);
  Vector2 facing = Vector2Scale(GetGuardFacing(g), GUARD_RADIUS);
  vertices[0] = Vector2Add(g->pos, Vector2Rotate(facing, PI));
  vertices[1] =
      Vector2Add(g->pos, Vector2Rotate(facing, GUARD_ANGLE_DIFF + PI));
  vertices[2] =
      Vector2Add(g->pos, Vector2Rotate(facing, GUARD_ANGLE_DIFF * 2 + PI));
  return vertices;
}

Vector2 *GetConeVertices(Guard *g) {
  Vector2 *v = GetVertices(g);
  // CONE
  Vector2 v1 = Vector2Add(
      v[0], Vector2Scale(Vector2Normalize(Vector2Subtract(v[1], v[0])),
                         GUARD_CONE_SIZE));
  Vector2 v2 = Vector2Add(
      v[0], Vector2Scale(Vector2Normalize(Vector2Subtract(v[2], v[0])),
                         GUARD_CONE_SIZE));
	Vector2 *vertices = malloc(sizeof(Vector2)*3);
	vertices[0] = v[0];
	vertices[1] = v1;
	vertices[2] = v2;
	return vertices;
}

void DrawGuard(Guard *g) {
  Vector2 *v = GetVertices(g);
  // CONE
  Vector2 v1 = Vector2Add(
      v[0], Vector2Scale(Vector2Normalize(Vector2Subtract(v[1], v[0])),
                         GUARD_CONE_SIZE));
  Vector2 v2 = Vector2Add(
      v[0], Vector2Scale(Vector2Normalize(Vector2Subtract(v[2], v[0])),
                         GUARD_CONE_SIZE));
  DrawTriangle(v2, v1, v[0], GUARD_CONE_COLOR);
  // BODY
  DrawTriangle(v[2], v[1], v[0], GUARD_COLOR);
  free(v);
}

void HandlePlayerMovement(Player *player, double delta) {
  Vector2 move = (Vector2){IsKeyDown(RIGHT_KEY) - IsKeyDown(LEFT_KEY),
                           IsKeyDown(DOWN_KEY) - IsKeyDown(UP_KEY)};
  // Linear Interpolation
  double lerp_weight =
      delta * ((move.x == 0 && move.y == 0) ? FRICTION : PLAYER_ACC);
  player->vel = Vector2Lerp(player->vel, Vector2Scale(move, PLAYER_MAX_SPEED),
                            lerp_weight);
  player->pos = Vector2Add(player->pos, player->vel);
}

bool CheckCollisionPlayerGuard(Player *p, Guard *g) {
  if (Vector2Distance(p->pos, g->pos) > GUARD_CONE_SIZE * 1.5)
    return false;
  Vector2 pVertices[4] = {
      p->pos,
      {p->pos.x + p->size.x, p->pos.y},
      {p->pos.x, p->pos.y + p->size.y},
      {p->pos.x + p->size.x, p->pos.y + p->size.y},
  };
  Vector2 *gVertices = GetConeVertices(g);
  DrawCircleV(gVertices[0], 5, ORANGE);
  DrawCircleV(gVertices[1], 5, ORANGE);
  DrawCircleV(gVertices[2], 5, ORANGE);
  // Cycle through the triangle sides
  for (int tIdx = 0; tIdx < 3; tIdx++) {
    // Cycle through the rectangle sides
    for (int rIdx = 0; rIdx < 4; rIdx++) {
      if (CheckCollisionLines(
              gVertices[tIdx], gVertices[(tIdx != 2) ? tIdx + 1 : 0],
              pVertices[rIdx], pVertices[(rIdx != 3) ? rIdx + 1 : 0], NULL)) {
        free(gVertices);
        return true;
      }
    }
  }

  free(gVertices);
  return false;
}

void UpdateGuard(Guard *g, int waypointIndex, double delta) {
  Vector2 targetPos = g->waypoints[waypointIndex];
  Vector2 facing = GetGuardFacing(g);
  Vector2 toTarget = Vector2Normalize(Vector2Subtract(targetPos, g->pos));

  float angleDiff = Vector2Angle(facing, toTarget);

  if (!g->walking) {
    // ROTATION PHASE
    if (fabsf(angleDiff) < (GUARD_ANGULAR_VEL * delta)) {
      g->walking = true; // Close enough to start walking
    } else {
      // Turn in the shortest direction
      float direction = (angleDiff > 0) ? 1.0f : -1.0f;
      g->facing += direction * GUARD_ANGULAR_VEL * delta;
    }
  } else {
    // MOVEMENT PHASE
    if (CheckCollisionPointCircle(g->pos, targetPos, 10)) {
      g->walking = false;
      g->currWaypoint = waypointIndex;
    } else {
      // Move forward based on current facing
      g->pos = Vector2Add(g->pos, Vector2Scale(facing, 2.0f));
    }
  }
}

void DrawGuardDebug(Guard *g, Vector2 target) {
  // 1. Draw a thin line to the target waypoint (The "Intention" line)
  DrawLineV(g->pos, target, Fade(YELLOW, 0.5f));

  // 2. Draw a bold line showing current facing (The "Nose" line)
  Vector2 facingDir = GetGuardFacing(g);
  Vector2 noseEnd = Vector2Add(g->pos, Vector2Scale(facingDir, 50));
  DrawLineEx(g->pos, noseEnd, 3.0f, RED);

  // 3. Draw a small circle at the waypoint to see the collision radius
  DrawCircleLinesV(target, 10, MAROON);
}
