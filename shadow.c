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

void DrawPlayer(Player *p) { DrawRectangleV(p->pos, p->size, PLAYER_COLOR); }

Vector2 *GetVertices(Guard *g) {
  Vector2 *vertices = malloc(sizeof(Vector2) * 3);
  Vector2 facing = Vector2Rotate(GUARD_SIZE_VEC, g->facing);
  vertices[0] = Vector2Add(g->pos, Vector2Rotate(facing, PI));
  vertices[1] =
      Vector2Add(g->pos, Vector2Rotate(facing, GUARD_ANGLE_DIFF + PI));
  vertices[2] =
      Vector2Add(g->pos, Vector2Rotate(facing, GUARD_ANGLE_DIFF * 2 + PI));
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

void HandlePlayerMovement(Player *player, Vector2 move, double delta) {
  // Linear Interpolation
  double lerp_weight =
      delta * ((move.x == 0 && move.y == 0) ? FRICTION : PLAYER_ACC);
  player->vel = Vector2Lerp(player->vel, Vector2Scale(move, PLAYER_MAX_SPEED),
                            lerp_weight);
}

bool CheckCollisionPlayerGuard(Player *p, Guard *g) {
  Vector2 pVertices[4] = {
      p->pos,
      {p->pos.x + p->size.x, p->pos.y},
      {p->pos.x, p->pos.y + p->size.y},
      {p->pos.x + p->size.x, p->pos.y + p->size.y},
  };

  Vector2 *gVertices = GetVertices(g);

  // Cycle through the triangle sides
  for (int t = 0; t < 3; t++) {
    // Cycle through the rectangle sides
    for (int r = 0; r < 4; r++) {
      if (CheckCollisionLines(gVertices[t], gVertices[(t != 2) ? t + 1 : 0],
                              pVertices[r], pVertices[(r != 3) ? r + 1 : 0],
                              NULL)) {
        free(gVertices);
        return true;
      }
    }
  }

  free(gVertices);
  return false;
}

void UpdateGuard(Guard *g, int waypoint, double delta) {
  Vector2 facing = Vector2Rotate(GUARD_SIZE_VEC, g->facing);
  if (!g->walking) {
    Vector2 waypointPos = g->waypoints[waypoint];
    Vector2 towardsWaypoint = Vector2Subtract(waypointPos, g->pos);
    double angle = Vector2Angle(towardsWaypoint, facing);
    if ((angle < 0)?-angle:angle <= GUARD_ANGULAR_VEL) {
      g->facing += angle;
      g->walking = true;
      return;
    }
    g->facing += ((angle > 0) ? -1 : 1) * GUARD_ANGULAR_VEL * delta;
  } else {
    g->pos = Vector2Add(g->pos, Vector2Scale(Vector2Normalize(facing), 10));
  }
}
