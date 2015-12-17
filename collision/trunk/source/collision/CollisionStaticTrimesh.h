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
#include "gamemath/Quaternionx.h"

struct trimeshVertex {
	Vector3x position;
	Vector3x normal;
	bool convex;
};

// Only convex edges are stored
struct trimeshConvexEdge {
	unsigned int vertexIndexes[2];
	Vector3x normal;
	Quaternionx planarTransform;
	fixed16_16 length;
};

struct trimeshTriangle {
	unsigned int vertexIndexes[3];
	unsigned int connectedTriangleIndexes[3];
	Vector3x normal;
};

#define CollisionStaticTrimesh_structContents(self_type) \
	CollisionObject_structContents(self_type) \
	\
	struct trimeshVertex * vertices; \
	unsigned int vertexCount; \
	struct trimeshConvexEdge * edges; \
	unsigned int edgeCount; \
	struct trimeshTriangle * triangles; \
	unsigned int triangleCount; \
	Box6x bounds;

stemobject_struct_definition(CollisionStaticTrimesh)

// vertices is interpreted like GL_TRIANGLES; no implicit sharing between triangles takes place. To share, use CollisionStaticTrimesh_createWithIndexes.
CollisionStaticTrimesh * CollisionStaticTrimesh_create(void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, unsigned int vertexCount);
bool CollisionStaticTrimesh_init(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, unsigned int vertexCount);

// indexes is interpreted like GL_TRIANGLES; no implicit index sharing between triangles takes place. Multiple triangles can index the same vertices.
CollisionStaticTrimesh * CollisionStaticTrimesh_createWithIndexes(void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, const unsigned int * indexes, unsigned int indexCount);
bool CollisionStaticTrimesh_initWithIndexes(CollisionStaticTrimesh * self, void * owner, CollisionCallback collisionCallback, const Vector3x * vertices, const unsigned int * indexes, unsigned int indexCount);

void CollisionStaticTrimesh_dispose(CollisionStaticTrimesh * self);

bool CollisionStaticTrimesh_isStatic(CollisionStaticTrimesh * self);
Box6x CollisionStaticTrimesh_getCollisionBounds(CollisionStaticTrimesh * self);

#ifdef __cplusplus
}
#endif
#endif
