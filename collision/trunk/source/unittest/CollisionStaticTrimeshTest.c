#include "collision/CollisionStaticTrimesh.h"
#include "collision/CollisionShared.h"
#include "unittest/TestSuite.h"
#include "utilities/printfFormats.h"
#include <limits.h>

static void verifyInit(int line, CollisionStaticTrimesh * trimesh, void * owner, CollisionCallback collisionCallback, Vector3x * vertices, unsigned int vertexCount) {
	unsigned int vertexIndex;
	
	TestCase_assert(trimesh != NULL, "Expected non-NULL but got NULL (line %d)", line);
	TestCase_assert(trimesh->dispose == CollisionStaticTrimesh_dispose, "Expected %p but got %p (line %d)", CollisionStaticTrimesh_dispose, trimesh->dispose, line);
	TestCase_assert(trimesh->shapeType == COLLISION_SHAPE_STATIC_TRIMESH, "Expected %d but got %d (line %d)", COLLISION_SHAPE_STATIC_TRIMESH, trimesh->shapeType, line);
	TestCase_assert(trimesh->owner == owner, "Expected %p but got %p (line %d)", owner, trimesh->owner, line);
	TestCase_assert(trimesh->collisionCallback == collisionCallback, "Expected %p but got %p (line %d)", collisionCallback, trimesh->collisionCallback, line);
	TestCase_assert(trimesh->isStatic == CollisionStaticTrimesh_isStatic, "Expected %p but got %p (line %d)", CollisionStaticTrimesh_isStatic, trimesh->isStatic, line);
	TestCase_assert(trimesh->getCollisionBounds == CollisionStaticTrimesh_getCollisionBounds, "Expected %p but got %p (line %d)", CollisionStaticTrimesh_getCollisionBounds, trimesh->getCollisionBounds, line);
	TestCase_assert(trimesh->vertexCount == vertexCount, "Expected %u but got %u (line %d)", vertexCount, trimesh->vertexCount, line);
	for (vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) {
		TestCase_assert(trimesh->vertices[vertexIndex].position.x == vertices[vertexIndex].x && trimesh->vertices[vertexIndex].position.y == vertices[vertexIndex].y && trimesh->vertices[vertexIndex].position.z == vertices[vertexIndex].z, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X} (line %d, vertex %u)", vertices[vertexIndex].x, vertices[vertexIndex].y, vertices[vertexIndex].z, trimesh->vertices[vertexIndex].position.x, trimesh->vertices[vertexIndex].position.y, trimesh->vertices[vertexIndex].position.z, line, vertexIndex);
	}
}

static void testInit() {
	CollisionStaticTrimesh trimesh, * trimeshPtr;
	Vector3x vertices[3] = {{0x00000, 0x10000, 0x20000}, {0x30000, 0x40000, 0x50000}, {0x60000, 0x70000, 0x80000}};
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_init(&trimesh, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0xFF, sizeof(trimesh));
	CollisionStaticTrimesh_init(&trimesh, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_init(&trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3);
	verifyInit(__LINE__, &trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	trimeshPtr = CollisionStaticTrimesh_create(NULL, NULL, NULL, 0);
	verifyInit(__LINE__, trimeshPtr, NULL, NULL, NULL, 0);
	CollisionStaticTrimesh_dispose(trimeshPtr);
	
	trimeshPtr = CollisionStaticTrimesh_create((void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3);
	verifyInit(__LINE__, trimeshPtr, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3);
	CollisionStaticTrimesh_dispose(trimeshPtr);
}

static void testInitWithIndexes() {
	CollisionStaticTrimesh trimesh, * trimeshPtr;
	Vector3x vertices[3] = {{0x60000, 0x70000, 0x80000}, {0x00000, 0x10000, 0x20000}, {0x30000, 0x40000, 0x50000}};
	unsigned int indexes[3] = {1, 2, 0};
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_initWithIndexes(&trimesh, NULL, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0xFF, sizeof(trimesh));
	CollisionStaticTrimesh_initWithIndexes(&trimesh, NULL, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, &trimesh, NULL, NULL, NULL, 0);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	memset(&trimesh, 0x00, sizeof(trimesh));
	CollisionStaticTrimesh_initWithIndexes(&trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, indexes, 3);
	verifyInit(__LINE__, &trimesh, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3);
	CollisionStaticTrimesh_dispose(&trimesh);
	
	trimeshPtr = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, NULL, NULL, 0);
	verifyInit(__LINE__, trimeshPtr, NULL, NULL, NULL, 0);
	CollisionStaticTrimesh_dispose(trimeshPtr);
	
	trimeshPtr = CollisionStaticTrimesh_createWithIndexes((void *) 0x1234, (CollisionCallback) 0x2345, vertices, indexes, 3);
	verifyInit(__LINE__, trimeshPtr, (void *) 0x1234, (CollisionCallback) 0x2345, vertices, 3);
	CollisionStaticTrimesh_dispose(trimeshPtr);
}

static void verifyVertex(struct trimeshVertex vertex, Vector3x position, Vector3x normal, bool convex, int line) {
	TestCase_assert(vertex.position.x == position.x && vertex.position.y == position.y && vertex.position.z == position.z, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X} (line %d)", position.x, position.y, position.z, vertex.position.x, vertex.position.y, vertex.position.z, line);
	TestCase_assert(vertex.normal.x == normal.x && vertex.normal.y == normal.y && vertex.normal.z == normal.z, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X} (line %d)", normal.x, normal.y, normal.z, vertex.normal.x, vertex.normal.y, vertex.normal.z, line);
	TestCase_assert(vertex.convex == convex, "Expected %s but got %s (line %d)", convex ? "true" : "false", vertex.convex ? "true" : "false", line);
}

static void verifyEdge(struct trimeshConvexEdge edge, unsigned int vertexIndex0, unsigned int vertexIndex1, Vector3x normal, int line) {
	TestCase_assert(edge.vertexIndexes[0] == vertexIndex0, "Expected %u but got %u (line %d)", vertexIndex0, edge.vertexIndexes[0], line);
	TestCase_assert(edge.vertexIndexes[1] == vertexIndex1, "Expected %u but got %u (line %d)", vertexIndex1, edge.vertexIndexes[1], line);
	TestCase_assert(edge.normal.x == normal.x && edge.normal.y == normal.y && edge.normal.z == normal.z, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X} (line %d)", normal.x, normal.y, normal.z, edge.normal.x, edge.normal.y, edge.normal.z, line);
}

static void verifyTriangle(struct trimeshTriangle triangle, unsigned int vertexIndex0, unsigned int vertexIndex1, unsigned int vertexIndex2, unsigned int connectedTriangleIndex0, unsigned int connectedTriangleIndex1, unsigned int connectedTriangleIndex2, Vector3x normal, int line) {
	TestCase_assert(triangle.vertexIndexes[0] == vertexIndex0, "Expected %u but got %u (line %d)", vertexIndex0, triangle.vertexIndexes[0], line);
	TestCase_assert(triangle.vertexIndexes[1] == vertexIndex1, "Expected %u but got %u (line %d)", vertexIndex1, triangle.vertexIndexes[1], line);
	TestCase_assert(triangle.vertexIndexes[2] == vertexIndex2, "Expected %u but got %u (line %d)", vertexIndex2, triangle.vertexIndexes[2], line);
	TestCase_assert(triangle.connectedTriangleIndexes[0] == connectedTriangleIndex0, "Expected %u but got %u (line %d)", connectedTriangleIndex0, triangle.connectedTriangleIndexes[0], line);
	TestCase_assert(triangle.connectedTriangleIndexes[1] == connectedTriangleIndex1, "Expected %u but got %u (line %d)", connectedTriangleIndex1, triangle.connectedTriangleIndexes[1], line);
	TestCase_assert(triangle.connectedTriangleIndexes[2] == connectedTriangleIndex2, "Expected %u but got %u (line %d)", connectedTriangleIndex2, triangle.connectedTriangleIndexes[2], line);
	TestCase_assert(triangle.normal.x == normal.x && triangle.normal.y == normal.y && triangle.normal.z == normal.z, "Expected {0x%05X, 0x%05X, 0x%05X} but got {0x%05X, 0x%05X, 0x%05X} (line %d)", normal.x, normal.y, normal.z, triangle.normal.x, triangle.normal.y, triangle.normal.z, line);
}

static void testGeometryInfo() {
	CollisionStaticTrimesh * trimesh;
	Vector3x vertices1[] = {{0x00000, 0x00000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x00000, 0x10000, 0x00000}, {0x10000, 0x10000, -0x10000}};
	Vector3x vertices2[] = {{0x00000, 0x00000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x00000, 0x10000, 0x00000}, {0x10000, 0x10000, 0x10000}};
	unsigned int indexes1[3] = {0, 1, 2}, indexes2[6] = {0, 1, 2, 2, 1, 3};
	
	trimesh = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, vertices1, indexes1, sizeof(indexes1) / sizeof(unsigned int));
	TestCase_assert(trimesh->vertexCount == 3, "Expected 3 but got %u", trimesh->vertexCount);
	verifyVertex(trimesh->vertices[0], VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000), false, __LINE__);
	verifyVertex(trimesh->vertices[1], VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000), false, __LINE__);
	verifyVertex(trimesh->vertices[2], VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000), false, __LINE__);
	TestCase_assert(trimesh->edgeCount == 0, "Expected 0 but got %u", trimesh->edgeCount);
	TestCase_assert(trimesh->triangleCount == 1, "Expected 1 but got %u", trimesh->triangleCount);
	verifyTriangle(trimesh->triangles[0], 0, 1, 2, UINT_MAX, UINT_MAX, UINT_MAX, VECTOR3x(0x00000, 0x00000, 0x10000), __LINE__);
	CollisionStaticTrimesh_dispose(trimesh);
	
	trimesh = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, vertices1, indexes2, sizeof(indexes2) / sizeof(unsigned int));
	TestCase_assert(trimesh->vertexCount == 4, "Expected 4 but got %u", trimesh->vertexCount);
	verifyVertex(trimesh->vertices[0], VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000), false, __LINE__);
	verifyVertex(trimesh->vertices[1], VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[2], VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[3], VECTOR3x(0x10000, 0x10000, -0x10000), VECTOR3x(0x093CE, 0x093CE, 0x093CE), false, __LINE__);
	TestCase_assert(trimesh->edgeCount == 1, "Expected 1 but got %u", trimesh->edgeCount);
	verifyEdge(trimesh->edges[0], 1, 2, VECTOR3x(0x05337, 0x05337, 0x0E359), __LINE__);
	TestCase_assert(trimesh->triangleCount == 2, "Expected 2 but got %u", trimesh->triangleCount);
	verifyTriangle(trimesh->triangles[0], 0, 1, 2, UINT_MAX, 1, UINT_MAX, VECTOR3x(0x00000, 0x00000, 0x10000), __LINE__);
	verifyTriangle(trimesh->triangles[1], 2, 1, 3, 0, UINT_MAX, UINT_MAX, VECTOR3x(0x093CD, 0x093CD, 0x093CD), __LINE__);
	CollisionStaticTrimesh_dispose(trimesh);
	
	trimesh = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, vertices2, indexes2, sizeof(indexes2) / sizeof(unsigned int));
	TestCase_assert(trimesh->vertexCount == 4, "Expected 4 but got %u", trimesh->vertexCount);
	verifyVertex(trimesh->vertices[0], VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000), false, __LINE__);
	verifyVertex(trimesh->vertices[1], VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(-0x05337, -0x05337, 0x0E359), false, __LINE__);
	verifyVertex(trimesh->vertices[2], VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(-0x05337, -0x05337, 0x0E359), false, __LINE__);
	verifyVertex(trimesh->vertices[3], VECTOR3x(0x10000, 0x10000, 0x10000), VECTOR3x(-0x093CE, -0x093CE, 0x093CE), false, __LINE__);
	TestCase_assert(trimesh->edgeCount == 0, "Expected 0 but got %u", trimesh->edgeCount);
	TestCase_assert(trimesh->triangleCount == 2, "Expected 2 but got %u", trimesh->triangleCount);
	verifyTriangle(trimesh->triangles[0], 0, 1, 2, UINT_MAX, 1, UINT_MAX, VECTOR3x(0x00000, 0x00000, 0x10000), __LINE__);
	verifyTriangle(trimesh->triangles[1], 2, 1, 3, 0, UINT_MAX, UINT_MAX, VECTOR3x(-0x093CD, -0x093CD, 0x093CD), __LINE__);
	CollisionStaticTrimesh_dispose(trimesh);
}

static void testVertexUniq() {
	CollisionStaticTrimesh * trimesh;
	Vector3x vertices[] = {{0x00000, 0x00000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x00000, 0x10000, 0x00000},
	                       {0x00000, 0x10000, 0x00000}, {0x10000, 0x00000, 0x00000}, {0x10000, 0x10000, -0x10000},
	                       {0x10000, 0x00000, 0x00000}, {0x00000, 0x00000, 0x00000}, {0x10000, 0x10000, -0x10000}};
	unsigned int indexes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices, 6);
	TestCase_assert(trimesh->vertexCount == 4, "Expected 4 but got %u", trimesh->vertexCount);
	verifyVertex(trimesh->vertices[0], VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000), false, __LINE__);
	verifyVertex(trimesh->vertices[1], VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[2], VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[3], VECTOR3x(0x10000, 0x10000, -0x10000), VECTOR3x(0x093CE, 0x093CE, 0x093CE), false, __LINE__);
	TestCase_assert(trimesh->triangleCount == 2, "Expected 2 but got %u", trimesh->triangleCount);
	verifyTriangle(trimesh->triangles[0], 0, 1, 2, UINT_MAX, 1, UINT_MAX, VECTOR3x(0x00000, 0x00000, 0x10000), __LINE__);
	verifyTriangle(trimesh->triangles[1], 2, 1, 3, 0, UINT_MAX, UINT_MAX, VECTOR3x(0x093CD, 0x093CD, 0x093CD), __LINE__);
	CollisionStaticTrimesh_dispose(trimesh);
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices, 9);
	TestCase_assert(trimesh->vertexCount == 4, "Expected 4 but got %u", trimesh->vertexCount);
	verifyVertex(trimesh->vertices[0], VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, -0x0EC84, 0x061F8), true, __LINE__);
	verifyVertex(trimesh->vertices[1], VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x08C73, -0x01F91, 0x0D3B3), true, __LINE__);
	verifyVertex(trimesh->vertices[2], VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[3], VECTOR3x(0x10000, 0x10000, -0x10000), VECTOR3x(0x0F3FB, -0x036D6, -0x036D6), true, __LINE__);
	TestCase_assert(trimesh->triangleCount == 3, "Expected 3 but got %u", trimesh->triangleCount);
	verifyTriangle(trimesh->triangles[0], 0, 1, 2, 2, 1, UINT_MAX, VECTOR3x(0x00000, 0x00000, 0x10000), __LINE__);
	verifyTriangle(trimesh->triangles[1], 2, 1, 3, 0, 2, UINT_MAX, VECTOR3x(0x093CD, 0x093CD, 0x093CD), __LINE__);
	verifyTriangle(trimesh->triangles[2], 1, 0, 3, 0, UINT_MAX, 1, VECTOR3x(0x00000, -0x0B505, -0x0B505), __LINE__);
	CollisionStaticTrimesh_dispose(trimesh);
	
	trimesh = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, vertices, indexes, 6);
	TestCase_assert(trimesh->vertexCount == 4, "Expected 4 but got %u", trimesh->vertexCount);
	verifyVertex(trimesh->vertices[0], VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, 0x00000, 0x10000), false, __LINE__);
	verifyVertex(trimesh->vertices[1], VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[2], VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[3], VECTOR3x(0x10000, 0x10000, -0x10000), VECTOR3x(0x093CE, 0x093CE, 0x093CE), false, __LINE__);
	TestCase_assert(trimesh->triangleCount == 2, "Expected 2 but got %u", trimesh->triangleCount);
	verifyTriangle(trimesh->triangles[0], 0, 1, 2, UINT_MAX, 1, UINT_MAX, VECTOR3x(0x00000, 0x00000, 0x10000), __LINE__);
	verifyTriangle(trimesh->triangles[1], 2, 1, 3, 0, UINT_MAX, UINT_MAX, VECTOR3x(0x093CD, 0x093CD, 0x093CD), __LINE__);
	CollisionStaticTrimesh_dispose(trimesh);
	
	trimesh = CollisionStaticTrimesh_createWithIndexes(NULL, NULL, vertices, indexes, 9);
	TestCase_assert(trimesh->vertexCount == 4, "Expected 4 but got %u", trimesh->vertexCount);
	verifyVertex(trimesh->vertices[0], VECTOR3x(0x00000, 0x00000, 0x00000), VECTOR3x(0x00000, -0x0EC84, 0x061F8), true, __LINE__);
	verifyVertex(trimesh->vertices[1], VECTOR3x(0x10000, 0x00000, 0x00000), VECTOR3x(0x08C73, -0x01F91, 0x0D3B3), true, __LINE__);
	verifyVertex(trimesh->vertices[2], VECTOR3x(0x00000, 0x10000, 0x00000), VECTOR3x(0x05337, 0x05337, 0x0E359), true, __LINE__);
	verifyVertex(trimesh->vertices[3], VECTOR3x(0x10000, 0x10000, -0x10000), VECTOR3x(0x0F3FB, -0x036D6, -0x036D6), true, __LINE__);
	TestCase_assert(trimesh->triangleCount == 3, "Expected 3 but got %u", trimesh->triangleCount);
	verifyTriangle(trimesh->triangles[0], 0, 1, 2, 2, 1, UINT_MAX, VECTOR3x(0x00000, 0x00000, 0x10000), __LINE__);
	verifyTriangle(trimesh->triangles[1], 2, 1, 3, 0, 2, UINT_MAX, VECTOR3x(0x093CD, 0x093CD, 0x093CD), __LINE__);
	verifyTriangle(trimesh->triangles[2], 1, 0, 3, 0, UINT_MAX, 1, VECTOR3x(0x00000, -0x0B505, -0x0B505), __LINE__);
	CollisionStaticTrimesh_dispose(trimesh);
}

static void testIsStatic() {
	CollisionStaticTrimesh * trimesh;
	bool result;
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, NULL, 0);
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
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices1, 3);
	bounds = CollisionStaticTrimesh_getCollisionBounds(trimesh);
	TestCase_assert(bounds.left == 0x00000 && bounds.right == 0x60000 &&
	                bounds.bottom == 0x10000 && bounds.top == 0x70000 &&
	                bounds.back == 0x20000 && bounds.front == 0x80000,
	                "Expected {0x00000, 0x60000, 0x10000, 0x70000, 0x20000, 0x80000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                bounds.left, bounds.right, bounds.bottom, bounds.top, bounds.back, bounds.front);
	CollisionStaticTrimesh_dispose(trimesh);
	
	trimesh = CollisionStaticTrimesh_create(NULL, NULL, vertices2, 6);
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
           testVertexUniq,
           testGeometryInfo,
           testIsStatic,
           testGetCollisionBounds)
