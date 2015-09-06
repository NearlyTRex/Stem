#include "collision/CollisionStaticTrimesh.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"

static void verifyInit(int line, CollisionStaticTrimesh * trimesh, void * owner, CollisionCallback collisionCallback, Vector3x * vertices, size_t vertexCount, bool copy) {
	size_t vertexIndex;
	
	TestCase_assert(trimesh != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(trimesh->dispose == CollisionStaticTrimesh_dispose, "Expected %p but got %p (line %d)", CollisionStaticTrimesh_dispose, trimesh->dispose, line);
	TestCase_assert(trimesh->shapeType == COLLISION_SHAPE_STATIC_TRIMESH, "Expected %d but got %d (line %d)", COLLISION_SHAPE_STATIC_TRIMESH, trimesh->shapeType, line);
	TestCase_assert(trimesh->owner == owner, "Expected %p but got %p (line %d)", owner, trimesh->owner, line);
	TestCase_assert(trimesh->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, trimesh->collisionCallback, line);
	TestCase_assert(trimesh->isStatic == CollisionStaticTrimesh_isStatic, "Expected %p but got %p (line %d)", CollisionStaticTrimesh_isStatic, trimesh->isStatic, line);
	TestCase_assert(trimesh->getCollisionBounds == CollisionStaticTrimesh_getCollisionBounds, "Expected %p but got %p (line %d)", CollisionStaticTrimesh_getCollisionBounds, trimesh->getCollisionBounds, line);
	TestCase_assert(trimesh->vertexCount == vertexCount, "Expected " SIZE_T_FORMAT " but got " SIZE_T_FORMAT " (line %d)", vertexCount, trimesh->vertexCount, line);
	if (copy) {
		TestCase_assert(trimesh->vertices != vertices, "Expected pointers to differ, but both were %p (line %d)", vertices, line);
	} else {
		TestCase_assert(trimesh->vertices == vertices, "Expected %p but got %p (line %d)", vertices, trimesh->vertices, line);
	}
	for (vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
		TestCase_assert(trimesh->vertices[vertexIndex].x == vertices[vertexIndex].x && trimesh->vertices[vertexIndex].y == vertices[vertexIndex].y && trimesh->vertices[vertexIndex].z == vertices[vertexIndex].z, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X} (line %d, vertex " SIZE_T_FORMAT ")", vertices[vertexIndex].x, vertices[vertexIndex].y, vertices[vertexIndex].z, trimesh->vertices[vertexIndex].x, trimesh->vertices[vertexIndex].y, trimesh->vertices[vertexIndex].z, line, vertexIndex);
	}
}

static void testInit() {
	CollisionStaticTrimesh trimesh, * trimeshPtr;
	Vector3x vertices[3] = {{0x00000, 0x10000, 0x20000}, {0x30000, 0x40000, 0x50000}, {0x60000, 0x70000, 0x80000}};
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_init(&trimesh, NULL, NULL, NULL, 0, false, false);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0, false);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0xFF, sizeof(trimesh));
	CollisionStaticTrimesh_init(&trimesh, NULL, NULL, NULL, 0, false, false);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0, false);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_init(&trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, false, false);
	verifyInit(__LINE__, &trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, false);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_init(&trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, true, false);
	verifyInit(__LINE__, &trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, true);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	trimeshPtr = CollisionStaticTrimesh_create(NULL, NULL, NULL, 0, false, false);
	verifyInit(__LINE__, trimeshPtr, NULL, NULL, NULL, 0, false);
	CollisionStaticTrimesh_dispose(trimeshPtr);
	
	trimeshPtr = CollisionStaticTrimesh_create((void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, false, false);
	verifyInit(__LINE__, trimeshPtr, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, false);
	CollisionStaticTrimesh_dispose(trimeshPtr);
	
	trimeshPtr = CollisionStaticTrimesh_create((void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, true, false);
	verifyInit(__LINE__, trimeshPtr, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3, true);
	CollisionStaticTrimesh_dispose(trimeshPtr);
}

static void testInitWithIndexes() {
	CollisionStaticTrimesh trimesh, * trimeshPtr;
	Vector3x verticesIndexed[3] = {{0x60000, 0x70000, 0x80000}, {0x00000, 0x10000, 0x20000}, {0x30000, 0x40000, 0x50000}};
	Vector3x verticesOrdered[4] = {{0x00000, 0x10000, 0x20000}, {0x30000, 0x40000, 0x50000}, {0x60000, 0x70000, 0x80000}, {0x30000, 0x40000, 0x50000}};
	unsigned int indexes[4] = {1, 2, 0, 2};
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_initWithIndexes(&trimesh, NULL, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0, false);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0xFF, sizeof(trimesh));
	CollisionStaticTrimesh_initWithIndexes(&trimesh, NULL, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0, false);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_initWithIndexes(&trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, verticesIndexed, indexes, 4);
	verifyInit(__LINE__, &trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, verticesOrdered, 4, true);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	trimeshPtr = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, trimeshPtr, NULL, NULL, NULL, 0, false);
	CollisionStaticTrimesh_dispose(trimeshPtr);
	
	trimeshPtr = CollisionStaticTrimesh_createWithIndexes((void *) 0x1234, (CollisionCallback) 0x2345, verticesIndexed, indexes, 4);
	verifyInit(__LINE__, trimeshPtr, (void *) 0x1234, (CollisionCallback) 0x2345, verticesOrdered, 4, true);
	CollisionStaticTrimesh_dispose(trimeshPtr);
}

static void testIsStatic() {
	CollisionStaticTrimesh * trimesh;
	bool result;
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, NULL, 0, false, false);
	result = CollisionStaticTrimesh_isStatic(trimesh);
	TestCase_assert(result, "Expected true but got false");
	CollisionStaticTrimesh_dispose(trimesh);
}

static void testGetCollisionBounds() {
	CollisionStaticTrimesh * trimesh;
	Vector3x vertices1[3] = {{0x00000, 0x10000, 0x20000}, {0x30000, 0x40000, 0x50000}, {0x60000, 0x70000, 0x80000}};
	Vector3x vertices2[6] = {{-0x30000, -0x20000, -0x10000}, {0x00000, 0x10000, 0x20000}, {0x30000, 0x40000, 0x50000},
	                         {-0x60000, -0x50000, -0x40000}, {-0x30000, -0x20000, -0x10000}, {0x00000, 0x10000, 0x20000}};
	Box6x bounds;
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices1, 3, false, false);
	bounds = CollisionStaticTrimesh_getCollisionBounds(trimesh);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x60000 &&
	                bounds.bottom == 0x10000 && bounds.top == 0x70000 &&
	                bounds.back == 0x20000 && bounds.front == 0x80000,
	                "Expected {0x00000, 0x60000, 0x10000, 0x70000, 0x20000, 0x80000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	CollisionStaticTrimesh_dispose(trimesh);
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices2, 6, false, false);
	bounds = CollisionStaticTrimesh_getCollisionBounds(trimesh);
	TestCase_assert(bounds.left == -0x60000 && bounds.right == 0x30000 &&
	                bounds.bottom == -0x50000 && bounds.top == 0x40000 &&
	                bounds.back == -0x40000 && bounds.front == 0x50000,
	                "Expected {0xFFFA0000, 0x30000, 0xFFFB0000, 0x40000, 0xFFFC0000, 0x50000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	CollisionStaticTrimesh_dispose(trimesh);
}

TEST_SUITE(CollisionStaticTrimeshTest,
           testInit,
           testInitWithIndexes,
           testIsStatic,
           testGetCollisionBounds)
