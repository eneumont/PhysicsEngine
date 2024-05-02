#include "body.h"
#include "integrator.h"
#include "world.h"

void Step(ncBody* body, float time) {
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));
	body->accleration = Vector2Scale(body->force, body->iMass);

	SemiExplicitEuler(body, time);

	//apply damping
	float damping = 1 / (1 + (body->damping * time));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}