#include "unittest/TestSuite.h"
#include "gamemath/Rect4d.h"
#include "gamemath/Rect4f.h"
#include "gamemath/Rect4i.h"
#include "gamemath/Rect4x.h"
#include "gamemath/Box6d.h"
#include "gamemath/Box6f.h"
#include "gamemath/Box6i.h"
#include "gamemath/Box6x.h"
#include "gamemath/RectBoxConversions.h"

#define assertRect4Equal(rect, expectedLeft, expectedRight, expectedBottom, expectedTop, format) \
	TestCase_assert(rect.left == expectedLeft && rect.right == expectedRight && rect.bottom == expectedBottom && rect.top == expectedTop, "Expected {" format ", " format ", " format ", " format "} but got {" format ", " format ", " format ", " format "}", expectedLeft, expectedRight, expectedBottom, expectedTop, rect.left, rect.right, rect.bottom, rect.top)
#define assertRect4dEqual(rect, expectedLeft, expectedRight, expectedBottom, expectedTop) \
	assertRect4Equal(rect, expectedLeft, expectedRight, expectedBottom, expectedTop, "%f")
#define assertRect4fEqual(rect, expectedLeft, expectedRight, expectedBottom, expectedTop) \
	assertRect4Equal(rect, expectedLeft, expectedRight, expectedBottom, expectedTop, "%f")
#define assertRect4iEqual(rect, expectedLeft, expectedRight, expectedBottom, expectedTop) \
	assertRect4Equal(rect, expectedLeft, expectedRight, expectedBottom, expectedTop, "%d")
#define assertRect4xEqual(rect, expectedLeft, expectedRight, expectedBottom, expectedTop) \
	assertRect4Equal(rect, expectedLeft, expectedRight, expectedBottom, expectedTop, "0x%05X")

#define assertBox6Equal(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront, format) \
	TestCase_assert(box.left == expectedLeft && box.right == expectedRight && box.bottom == expectedBottom && box.top == expectedTop && box.back == expectedBack && box.front == expectedFront, "Expected {" format ", " format ", " format ", " format ", " format ", " format "} but got {" format ", " format ", " format ", " format ", " format ", " format "}", expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront, box.left, box.right, box.bottom, box.top, box.back, box.front)
#define assertBox6dEqual(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront) \
	assertBox6Equal(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront, "%f")
#define assertBox6fEqual(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront) \
	assertBox6Equal(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront, "%f")
#define assertBox6iEqual(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront) \
	assertBox6Equal(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront, "%d")
#define assertBox6xEqual(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront) \
	assertBox6Equal(box, expectedLeft, expectedRight, expectedBottom, expectedTop, expectedBack, expectedFront, "0x%05X")

static void testTypeConversions() {
	Rect4d rect4d;
	Rect4f rect4f;
	Rect4i rect4i;
	Rect4x rect4x;
	Box6d box6d;
	Box6f box6f;
	Box6i box6i;
	Box6x box6x;
	
	rect4d = Rect4f_toRect4d(RECT4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertRect4dEqual(rect4d, 1.0, 2.0, 3.0, 4.0);
	rect4d = Rect4i_toRect4d(RECT4i(1, 2, 3, 4));
	assertRect4dEqual(rect4d, 1.0, 2.0, 3.0, 4.0);
	rect4d = Rect4x_toRect4d(RECT4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertRect4dEqual(rect4d, 1.0, 2.0, 3.0, 4.0);
	
	rect4f = Rect4d_toRect4f(RECT4d(1.0, 2.0, 3.0, 4.0));
	assertRect4fEqual(rect4f, 1.0f, 2.0f, 3.0f, 4.0f);
	rect4f = Rect4i_toRect4f(RECT4i(1, 2, 3, 4));
	assertRect4fEqual(rect4f, 1.0f, 2.0f, 3.0f, 4.0f);
	rect4f = Rect4x_toRect4f(RECT4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertRect4fEqual(rect4f, 1.0f, 2.0f, 3.0f, 4.0f);
	
	rect4i = Rect4d_toRect4i(RECT4d(1.0, 2.0, 3.0, 4.0));
	assertRect4iEqual(rect4i, 1, 2, 3, 4);
	rect4i = Rect4f_toRect4i(RECT4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertRect4iEqual(rect4i, 1, 2, 3, 4);
	rect4i = Rect4x_toRect4i(RECT4x(0x10000, 0x20000, 0x30000, 0x40000));
	assertRect4iEqual(rect4i, 1, 2, 3, 4);
	
	rect4x = Rect4d_toRect4x(RECT4d(1.0, 2.0, 3.0, 4.0));
	assertRect4xEqual(rect4x, 0x10000, 0x20000, 0x30000, 0x40000);
	rect4x = Rect4f_toRect4x(RECT4f(1.0f, 2.0f, 3.0f, 4.0f));
	assertRect4xEqual(rect4x, 0x10000, 0x20000, 0x30000, 0x40000);
	rect4x = Rect4i_toRect4x(RECT4i(1, 2, 3, 4));
	assertRect4xEqual(rect4x, 0x10000, 0x20000, 0x30000, 0x40000);
	
	
	box6d = Box6f_toBox6d(BOX6f(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f));
	assertBox6dEqual(box6d, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
	box6d = Box6i_toBox6d(BOX6i(1, 2, 3, 4, 5, 6));
	assertBox6dEqual(box6d, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
	box6d = Box6x_toBox6d(BOX6x(0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000));
	assertBox6dEqual(box6d, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
	
	box6f = Box6d_toBox6f(BOX6d(1.0, 2.0, 3.0, 4.0, 5.0, 6.0));
	assertBox6fEqual(box6f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
	box6f = Box6i_toBox6f(BOX6i(1, 2, 3, 4, 5, 6));
	assertBox6fEqual(box6f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
	box6f = Box6x_toBox6f(BOX6x(0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000));
	assertBox6fEqual(box6f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
	
	box6i = Box6d_toBox6i(BOX6d(1.0, 2.0, 3.0, 4.0, 5.0, 6.0));
	assertBox6iEqual(box6i, 1, 2, 3, 4, 5, 6);
	box6i = Box6f_toBox6i(BOX6f(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f));
	assertBox6iEqual(box6i, 1, 2, 3, 4, 5, 6);
	box6i = Box6x_toBox6i(BOX6x(0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000));
	assertBox6iEqual(box6i, 1, 2, 3, 4, 5, 6);
	
	box6x = Box6d_toBox6x(BOX6d(1.0, 2.0, 3.0, 4.0, 5.0, 6.0));
	assertBox6xEqual(box6x, 0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000);
	box6x = Box6f_toBox6x(BOX6f(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f));
	assertBox6xEqual(box6x, 0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000);
	box6x = Box6i_toBox6x(BOX6i(1, 2, 3, 4, 5, 6));
	assertBox6xEqual(box6x, 0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000);
}

TEST_SUITE(RectBoxConversionsTest,
           testTypeConversions)
