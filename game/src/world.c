#include "world.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncbodyCount = 0;

ncBody* CreateBody() {
	ncBody* b = (ncBody*)malloc(sizeof(ncBody));
	assert(b);

	memset(b, 0, sizeof(ncBody));

	b->prev = NULL;
	b->next = ncBodies;

	if (ncBodies != NULL) {
		ncBodies->prev = b;
	}
	ncBodies = b;

	ncbodyCount++;
	return b;
}

void DestroyBody(ncBody* body) {
	assert(body);

	if (body->prev != NULL) body->prev->next = body->next;

	if (body->next != NULL) body->next->prev = body->next;

	if (body == ncBodies) ncBodies = body->next;

	ncbodyCount--;
	free(body);
}

void DestroyAllBodies() {

}