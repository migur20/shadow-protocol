#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

float MyVector2Angle(Vector2 v1, Vector2 v2){
	return ( cos( (v1.x*v2.x + v1.y*v2.y) / Vector2Length(v1)*Vector2Length(v2) ) );
}

int main(){
	const int width = 800;
	const int height = 800;
	InitWindow(width, height, "Demo");
	SetTargetFPS(60);

	float angle1 = 0;
	float angle2 = PI/2;

	Vector2 pos = {(float)width/2, (float)height/2};

	const Vector2 facingZero = {1, 0};

	Vector2 facingVec1;
	Vector2 facingVec2;
	float delta;
	while(!WindowShouldClose()){
		delta = GetFrameTime();

		facingVec1 = Vector2Normalize(Vector2Rotate(facingZero, angle1));
		printf("facingVec1:%f,%f\n", facingVec1.x, facingVec1.y);

		facingVec2 = Vector2Normalize(Vector2Rotate(facingZero, angle2));
		printf("facingVec2:%f,%f\n", facingVec2.x, facingVec2.y);

		angle1 += 60*DEG2RAD * delta;
		if(angle1 >= 2*PI)
			angle1 = 0;
		//printf("angle:%f\n", angle1*RAD2DEG);

		angle2 += 60*DEG2RAD * delta;
		if(angle2 >= 2*PI)
			angle2 = 0;
		//printf("angle:%f\n", angle2*RAD2DEG);

		printf("Angle between vectors: %f\n", RAD2DEG*Vector2Angle(facingVec1, facingVec2));
		printf("My Angle between vectors: %f\n", MyVector2Angle(facingVec1, facingVec2));
		printf("Angle between angles: %f\n", RAD2DEG *(angle1 - angle2));

		BeginDrawing();{
			ClearBackground(DARKGRAY);
			DrawCircleV(pos, 50, RED);
			DrawLineEx(pos, Vector2Add(pos, Vector2Scale(facingVec1, 100)), 20, SKYBLUE);
			DrawLineEx(pos, Vector2Add(pos, Vector2Scale(facingVec2, 100)), 20, SKYBLUE);
		}EndDrawing();
	}

	CloseWindow();
	return EXIT_SUCCESS;
}
