#include <raylib.h>
#include <raymath.h>
#include <math.h>

#define FPS 60

typedef enum {MENU, GAME, GAME_OVER} GAME_STATE;

#define UP_KEY    KEY_W
#define LEFT_KEY  KEY_A
#define DOWN_KEY  KEY_S
#define RIGHT_KEY KEY_D

#define BG (Color){0x11, 0x12, 0x21, 0xFF}

#define FRICTION 8.0
#define PLAYER_MAX_SPEED 10.0
#define PLAYER_ACC 3.0

#define PLAYER_COLOR (Color){160, 45, 80, 0xFF}
#define PLAYER_SPEED 7.0
#define PLAYER_RADIUS 20.0

typedef struct{
	Vector2 pos; //Top left corner of the rect
	Vector2 vel; //Direction and velocity the player is moving
	Vector2 size;//Rectangle dimendions
	bool hasChip;
}Player;

#define GUARD_COLOR GREEN
#define GUARD_ANGULAR_VEL PI/4
#define GUARD_RADIUS 15.0
#define GUARD_ANGLE_DIFF (PI*2.0)/3.0  // "(PI*2.0)3.0" for equilateral triangle
#define GUARD_CONE_SIZE 125
#define GUARD_CONE_COLOR (Color){253, 249, 0, 50}
#define FACING_ZERO_VEC (Vector2){1, 0}

typedef struct{
	Vector2 pos;         //Middle of the triangle
	double facing;
	Vector2 waypoints[2];
  int currWaypoint;
	bool walking;
}Guard;

#define CHIP_COLOR SKYBLUE
#define CHIP_RADIUS 5

void DrawPlayer(Player *p);

void DrawGuard(Guard *g);

void UpdateGuard(Guard *g, int waypoint, double delta);

void HandlePlayerMovement(Player *player, double delta);

void Log(Player player);

bool CheckCollisionPlayerGuard(Player *p, Guard *e);

void DrawGuardDebug(Guard *g, Vector2 target);
