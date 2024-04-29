#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void) {
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	Body* bodies = (Body*)malloc(sizeof(Body) * MAX_BODIES);
	assert(bodies);

	int bodyCount = 0;

	while (!WindowShouldClose()) {
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if ((IsMouseButtonDown(0))) {
			bodies[bodyCount].position = position;
			bodies[bodyCount].velocity = createVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
			bodyCount++;
		}

		//render
		BeginDrawing();
		ClearBackground(BLACK);
		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 10, PURPLE);

		//update bodies
		for (int i = 0; i < bodyCount; i++) {
			bodies[i].position = Vector2Add(bodies[i].position, bodies[i].velocity);
			DrawCircle((int)bodies[i].position.x, (int)bodies[i].position.y, 10, BLUE);
		}

		EndDrawing();
	}

	CloseWindow();
	free(bodies);

	return 0;
}