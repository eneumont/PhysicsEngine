#pragma once
#include "raylib.h"

//struct for just values
typedef struct Body {
	//force -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;
} Body;