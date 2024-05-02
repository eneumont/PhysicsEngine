#pragma once
#include "body.h"

inline void ExplicitEuler(ncBody* body, float timestep) {
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->accleration, timestep));
}

inline void SemiExplicitEuler(ncBody* body, float timestep) {
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->accleration, timestep));
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
}