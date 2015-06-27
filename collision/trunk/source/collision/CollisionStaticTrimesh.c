/*
  Copyright (c) 2015 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com
*/

#include "collision/CollisionStaticTrimesh.h"
#include "collision/CollisionShared.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS CollisionObject

CollisionStaticTrimesh * CollisionStaticTrimesh_create(void * owner, CollisionCallback collisionCallback, Vector3x * vertices, size_t vertexCount, bool copy, bool takeOwnership) {
	stemobject_create_implementation(CollisionStaticTrimesh, init, owner, collisionCallback, vertices, vertexCount, copy, takeOwnership)
}

static void sharedInit(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback) {
	call_super(init, self, owner, COLLISION_SHAPE_STATIC_TRIMESH, collisionCallback);
	self->dispose = CollisionStaticTrimesh_dispose;
}

bool CollisionStaticTrimesh_init(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, Vector3x * vertices, size_t vertexCount, bool copy, bool takeOwnership) {
	sharedInit(self, owner, collisionCallback);
	self->vertexCount = vertexCount;
	if (copy) {
		self->vertices = malloc(sizeof(Vector3x) * vertexCount);
		memcpy(self->vertices, vertices, sizeof(Vector3x) * vertexCount);
	} else {
		self->vertices = vertices;
	}
	self->private_ivar(verticesOwned) = takeOwnership;
	return true;
}

CollisionStaticTrimesh * CollisionStaticTrimesh_createWithIndexes(void * owner, CollisionCallback collisionCallback, Vector3x * vertices, unsigned int * indexes, size_t indexCount) {
	stemobject_create_implementation(CollisionStaticTrimesh, initWithIndexes, owner, collisionCallback, vertices, indexes, indexCount)
}

bool CollisionStaticTrimesh_initWithIndexes(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, Vector3x * vertices, unsigned int * indexes, size_t indexCount) {
	sharedInit(self, owner, collisionCallback);
	self->vertexCount = indexCount;
	if (indexCount == 0) {
		self->vertices = NULL;
	} else {
		size_t index;
		
		self->vertices = malloc(sizeof(Vector3x) * indexCount);
		for (index = 0; index < indexCount; index++) {
			self->vertices[index] = vertices[indexes[index]];
		}
	}
	return true;
}

void CollisionStaticTrimesh_dispose(CollisionStaticTrimesh * self) {
	if (self->private_ivar(verticesOwned)) {
		free(self->vertices);
	}
	call_super(dispose, self);
}
