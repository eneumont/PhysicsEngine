#include "spring.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k) {
	ncSpring_t* s = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(s);

	memset(s, 0, sizeof(ncSpring_t));
	s->body1 = body1;
	s->body2 = body2;
	s->restLength = restLength;
	s->k = k;

	return s;
}

void AddSpring(ncSpring_t* s) {
	assert(s);

	s->prev = NULL;
	s->next = ncSprings;

	if (ncSprings) {
		ncSprings->prev = s;
	}
	ncSprings = s;
}

void DestroySpring(ncSpring_t* s) {
	assert(s);

	if (s->prev) s->prev->next = s->next;
	if (s->next) s->next->prev = s->prev;

	if (s == ncSprings) ncSprings = s->next;

	free(s);
}

void DestroyAllSprings() {

}

void ApplySpringForce(ncSpring_t* springs) {
	for (ncSpring_t* spring = springs; spring; spring = spring->next) {
		Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position);
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = length - spring->restLength;
		float force = -spring->k * x; // f = -kx <- hooke's law

		Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(Vector2Negate(ndirection), force), FM_FORCE); //could also use -force instead
	}
}

void ApplySpringForcePosition(Vector2 position, struct ncBody* body, float restLength, float k, float damping) {
	// Check if the body pointer is null; if so, exit the function
	if (!body) return;

	// Calculate the direction vector from the body's position to the given position
	Vector2 direction = Vector2Subtract(position, body->position);

	// If the direction vector is zero (i.e., positions are the same), exit the function
	if (direction.x == 0 && direction.y == 0) return;

	// Calculate the length of the direction vector (distance between the two positions)
	float length = Vector2Length(direction);

	// Calculate the displacement from the rest length
	float x = length - restLength;

	// Apply Hooke's Law (f = -kx) to determine the spring force
	float force = -k * x;

	// Normalize the direction vector
	Vector2 ndirection = Vector2Normalize(direction);

	// Calculate the damping force (opposing force due to velocity)
	float dampingForce = damping * Vector2DotProduct(body->velocity, ndirection); //<damping * DotProduct(body velocity, ndirection)>

	// Calculate the total force by combining spring force and damping force
	float totalForce = force + dampingForce; //<force + damping force>

	// Apply the total force to the body in the direction of the normalized vector
	ApplyForce(body, Vector2Scale(ndirection, -totalForce)/*<scale the normalized direction vector by - totalForce >*/, FM_FORCE);
}