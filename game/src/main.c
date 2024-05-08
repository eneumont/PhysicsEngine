#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "integrator.h"
#include "world.h"
#include "force.h"
#include "render.h"
#include "editor.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(void) {
	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		//initialize world
		ncGravity = (Vector2){ 0, -1 };

		Vector2 position = GetMousePosition();
		ncScreenZoom -= GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);

		UpdateEditor(position);

		if ((IsMouseButtonDown(0))) {
			ncBody* body = CreateBody();
			body->position = ConvertScreenToWorld(position);
			body->mass = GetRandomFloatValue(ncEditorData.SliderBar001Value, ncEditorData.SliderBar002Value);
			body->iMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.1f;
			body->gravityScale = 10;
			body->color = ColorFromHSV( GetRandomFloatValue(0, 360), 1, 1);
			
			//ApplyForce(body, (Vector2){GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
		}

		/*if ((IsMouseButtonDown(0)) && IsKeyDown(KEY_X)) {
			ncBody* body = CreateBody();
			body->position = position;
			body->mass = GetRandomFloatValue(3, 10);
			body->iMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.1f;
			body->gravityScale = 10;
			body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

			Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(1000, 2000));

			ApplyForce(body, force, FM_IMPULSE);
		}

		if ((IsMouseButtonDown(0)) && IsKeyDown(KEY_C)) {
			ncBody* body = CreateBody();
			body->position = position;
			body->mass = GetRandomFloatValue(3, 10);
			body->iMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.1f;
			body->gravityScale = 10;
			body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);

			float angle = GetRandomFloatValue(250, 280);
			Vector2 force = Vector2Scale(GetVector2FromAngle((angle + GetRandomFloatValue(-30, 30)) * DEG2RAD), GetRandomFloatValue(1000, 5000));
			ApplyForce(body, force, FM_IMPULSE);
		}

		if ((IsMouseButtonDown(0)) && IsKeyDown(KEY_V)) {
			ncBody* body = CreateBody();
			body->position = Vector2Subtract(position, (Vector2){ 0, 60 });
			body->mass = GetRandomFloatValue(3, 10);
			body->iMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 2.1f;
			body->gravityScale = 10;
			body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
			Vector2 force = Vector2Scale(GetVector2FromAngle(GetRandomFloatValue(0, 360) * DEG2RAD), GetRandomFloatValue(10, 20));
			ApplyForce(body, force, FM_VELOCITY);

			ncBody* body1 = CreateBody();
			body1->position = Vector2Subtract(position, (Vector2) { 60, -60 });
			body1->mass = GetRandomFloatValue(3, 10);
			body1->iMass = 1 / body1->mass;
			body1->type = BT_DYNAMIC;
			body1->damping = 2.1f;
			body1->gravityScale = 10;
			body1->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
			ApplyForce(body1, force, FM_VELOCITY);

			ncBody* body2 = CreateBody();
			body2->position = Vector2Subtract(position, (Vector2) { -60, -60 });
			body2->mass = GetRandomFloatValue(3, 10);
			body2->iMass = 1 / body2->mass;
			body2->type = BT_DYNAMIC;
			body2->damping = 2.1f;
			body2->gravityScale = 10;
			body2->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
			ApplyForce(body2, force, FM_VELOCITY);
		}*/

		//apply force
		ApplyGravity(ncBodies, ncEditorData.SliderBar003Value);

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
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass), body->color);
			//ClearForce(body);
		}

		DrawEditor();
		
		EndDrawing();
	}

	CloseWindow();
	free(ncBodies);

	return 0;
}