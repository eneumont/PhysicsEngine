#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "integrator.h"
#include "world.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(void) {
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		//update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		//initialize world
		ncGravity = (Vector2){ 0, ncEditorData.GravityValue };

		Vector2 position = GetMousePosition();
		ncScreenZoom -= GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody) {
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		if (!ncEditorIntersect) {
			//create body
			if ((IsMouseButtonPressed(0) || (IsMouseButtonDown(0) && IsKeyDown(KEY_LEFT_CONTROL)))) {
				ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassValue, ncEditorData.BodyTypeActive);
				body->damping = ncEditorData.DampingValue;
				body->gravityScale = ncEditorData.GravityScaleValue;
				body->color = WHITE;//ColorFromHSV( GetRandomFloatValue(0, 360), 1, 1);
				body->restitution = 0.8f;

				//ApplyForce(body, (Vector2){GetRandomFloatValue(-200, 200), GetRandomFloatValue(-200, 200) }, FM_VELOCITY);
				AddBody(body);
			}

			//connect springs
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
			if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody) {
				if (selectedBody && selectedBody != connectBody) {
					ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
					AddSpring(spring);
				}
			}
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
		ApplyGravity(ncBodies, ncEditorData.GravitationValue);
		ApplySpringForce(ncSprings);

		//update bodies
		for (ncBody* body = ncBodies; body; body = body->next) {
			Step(body, dt);
		}

		//collision
		ncContact_t* contacts = NULL;
		CreateContacts(ncBodies, &contacts);
		SeparateContacts(contacts);
		ResolveContacts(contacts);

		//render
		BeginDrawing();
		ClearBackground(BLACK);

		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt), 10, 30, 20, LIME);

		//draw bodies
		for (ncBody* body = ncBodies; body; body = body->next) {
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}

		//draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next) {
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), BLUE);
		}

		//draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next) {
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, RED);
		}

		DrawEditor(position);
		
		EndDrawing();
	}

	CloseWindow();
	free(ncBodies);

	return 0;
}