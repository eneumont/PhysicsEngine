#include "spring.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;
int ncSpring_tCount = 0;
Vector2 ncGravity;

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

	ncSpring_tCount++;
}

void DestroySpring(ncSpring_t* s) {
	assert(s);

	if (s->prev) s->prev->next = s->next;
	if (s->next) s->next->prev = s->prev;

	if (s == ncSprings) ncSprings = s->next;

	ncSpring_tCount--;
	free(s);
}

void DestroyAllSprings() {

}

void ApplySpringForce(ncSpring_t* springs) {

}