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

#ifndef __CollisionStaticTrimesh_H__
#define __CollisionStaticTrimesh_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct CollisionStaticTrimesh CollisionStaticTrimesh;

#include "collision/CollisionObject.h"
#include "gamemath/Vector3x.h"

#define CollisionStaticTrimesh_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	Vector3x * vertices; \
	bool private_ivar(verticesOwned); \
	size_t vertexCount;

stemobject_struct_definition(CollisionStaticTrimesh)

// vertices is interpreted like GL_TRIANGLES; no implicit sharing between triangles takes place. To share, use CollisionStaticTrimesh_createWithIndexes.
// If copy is true, a new copy of vertices will be stores in this CollisionStaticTrimesh. If false, the original pointer will be used.
// If takeOwnership is true, vertices will be free when this CollisionStaticTrimesh is disposed.
CollisionStaticTrimesh * CollisionStaticTrimesh_create(void * owner, CollisionCallback collisionCallback, Vector3x * vertices, size_t vertexCount, bool copy, bool takeOwnership);
bool CollisionStaticTrimesh_init(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, Vector3x * vertices, size_t vertexCount, bool copy, bool takeOwnership);

// indexes is interpreted like GL_TRIANGLES; no implicit index sharing between triangles takes place. Multiple triangles can index the same vertices.
// This init function implies copy and takeOwnership, so the vertices and indexes pointers passed to this function will not be freed by CollisionStaticTrimesh.
CollisionStaticTrimesh * CollisionStaticTrimesh_createWithIndexes(void * owner, CollisionCallback collisionCallback, Vector3x * vertices, unsigned int * indexes, size_t indexCount);
bool CollisionStaticTrimesh_initWithIndexes(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, Vector3x * vertices, unsigned int * indexes, size_t indexCount);

void CollisionStaticTrimesh_dispose(CollisionStaticTrimesh * self);

#ifdef __cplusplus
}
#endif
#endif