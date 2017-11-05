#include "glgraphics/Renderable.h"
#include "unittest/TestSuite.h"

static void testInit() {
	Renderable renderable;
	
	memset(&renderable, 0x00, sizeof(Renderable));
	Renderable_init(&renderable, RENDERABLE_MESH);
	TestCase_assert(renderable.type == RENDERABLE_MESH, "Expected %d but got %d", RENDERABLE_MESH, renderable.type);
	TestCase_assert(renderable.dispose == Renderable_dispose, "Expected %p but got %p", Renderable_dispose, renderable.dispose);
	Renderable_dispose(&renderable);
	
	memset(&renderable, 0xFF, sizeof(Renderable));
	Renderable_init(&renderable, RENDERABLE_MESH);
	TestCase_assert(renderable.type == RENDERABLE_MESH, "Expected %d but got %d", RENDERABLE_MESH, renderable.type);
	TestCase_assert(renderable.dispose == Renderable_dispose, "Expected %p but got %p", Renderable_dispose, renderable.dispose);
	Renderable_dispose(&renderable);
	
	memset(&renderable, 0x00, sizeof(Renderable));
	Renderable_init(&renderable, RENDERABLE_DYNAMIC_MESH);
	TestCase_assert(renderable.type == RENDERABLE_DYNAMIC_MESH, "Expected %d but got %d", RENDERABLE_DYNAMIC_MESH, renderable.type);
	TestCase_assert(renderable.dispose == Renderable_dispose, "Expected %p but got %p", Renderable_dispose, renderable.dispose);
	Renderable_dispose(&renderable);
}

TEST_SUITE(RenderableTest, testInit)
