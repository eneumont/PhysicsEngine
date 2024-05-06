#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "integrator.h"
#include "world.h"
#include "force.h"

#include <stdlib.h>
#include <assert.h>

int main(void) {
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		//initialize world
		//ncGravity = (Vector2){ 0, 0 };

		Vector2 position = GetMousePosition();
		if ((IsMouseButtonDown(0))) {
			ncBody* body = CreateBody();
			body->position = position;
			body->mass = GetRandomFloatValue(3, 10);
			body->iMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.1f;
			body->gravityScale = 10;
			body->color = ColorFromHSV( GetRandomFloatValue(0, 360), 1, 1);
			ApplyForce(body, (Vector2){GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
		}

		//apply force
		ApplyGravity(ncBodies, 30);

		for (ncBody* body = ncBodies; body; body = body->next) {
			Step(body, dt);
		}

		//render
		BeginDrawing();
		ClearBackground(BLACK);
		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 10, PURPLE);

		//draw bodies
		for (ncBody* body = ncBodies; body; body = body->next) {
			DrawCircle((int)body->position.x, (int)body->position.y, body->mass, body->color);
			ClearForce(body);
		}

		EndDrawing();
	}

	CloseWindow();
	free(ncBodies);

	return 0;
}