#include "unittest/TestSuite.h"
#include "gamemath/Rect4d.h"
#include "gamemath/Rect4f.h"
#include "gamemath/Rect4i.h"
#include "gamemath/Rect4x.h"
#include "gamemath/Box6d.h"
#include "gamemath/Box6f.h"
#include "gamemath/Box6i.h"
#include "gamemath/Box6x.h"

static void testConstructor() {
	Rect4d rect4d;
	Rect4f rect4f;
	Rect4i rect4i;
	Rect4x rect4x;
	Box6d box6d;
	Box6f box6f;
	Box6i box6i;
	Box6x box6x;
	
	rect4d = RECT4d(0.0, 0.0, 0.0, 0.0);
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 0.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = RECT4d(1.0, 2.0, 3.0, 4.0);
	TestCase_assert(rect4d.left == 1.0 &&
	                rect4d.right == 2.0 &&
	                rect4d.bottom == 3.0 &&
	                rect4d.top == 4.0,
	                "Expected {1.0, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	
	rect4f = RECT4f(0.0f, 0.0f, 0.0f, 0.0f);
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 0.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = RECT4f(1.0f, 2.0f, 3.0f, 4.0f);
	TestCase_assert(rect4f.left == 1.0f &&
	                rect4f.right == 2.0f &&
	                rect4f.bottom == 3.0f &&
	                rect4f.top == 4.0f,
	                "Expected {1.0, 2.0, 3.0, 4.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	
	rect4i = RECT4i(0, 0, 0, 0);
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 0 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 0,
	                "Expected {0, 0, 0, 0} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = RECT4i(1, 2, 3, 4);
	TestCase_assert(rect4i.left == 1 &&
	                rect4i.right == 2 &&
	                rect4i.bottom == 3 &&
	                rect4i.top == 4,
	                "Expected {1, 2, 3, 4} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	
	rect4x = RECT4x(0x00000, 0x00000, 0x00000, 0x00000);
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x00000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = RECT4x(0x10000, 0x20000, 0x30000, 0x40000);
	TestCase_assert(rect4x.left == 0x10000 &&
	                rect4x.right == 0x20000 &&
	                rect4x.bottom == 0x30000 &&
	                rect4x.top == 0x40000,
	                "Expected {0x10000, 0x20000, 0x30000, 0x40000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	
	box6d = BOX6d(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 0.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 0.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = BOX6d(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
	TestCase_assert(box6d.left == 1.0 &&
	                box6d.right == 2.0 &&
	                box6d.bottom == 3.0 &&
	                box6d.top == 4.0 &&
	                box6d.back == 5.0 &&
	                box6d.front == 6.0,
	                "Expected {1.0, 2.0, 3.0, 4.0, 5.0, 6.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	
	box6f = BOX6f(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 0.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 0.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = BOX6f(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);
	TestCase_assert(box6f.left == 1.0f &&
	                box6f.right == 2.0f &&
	                box6f.bottom == 3.0f &&
	                box6f.top == 4.0f &&
	                box6f.back == 5.0f &&
	                box6f.front == 6.0f,
	                "Expected {1.0, 2.0, 3.0, 4.0, 5.0, 6.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	
	box6i = BOX6i(0, 0, 0, 0, 0, 0);
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 0 &&
	                box6i.bottom == 0 &&
	                box6i.top == 0 &&
	                box6i.back == 0 &&
	                box6i.front == 0,
	                "Expected {0, 0, 0, 0, 0, 0} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = BOX6i(1, 2, 3, 4, 5, 6);
	TestCase_assert(box6i.left == 1 &&
	                box6i.right == 2 &&
	                box6i.bottom == 3 &&
	                box6i.top == 4 &&
	                box6i.back == 5 &&
	                box6i.front == 6,
	                "Expected {1, 2, 3, 4, 5, 6} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	
	box6x = BOX6x(0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000);
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x00000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x00000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = BOX6x(0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000);
	TestCase_assert(box6x.left == 0x10000 &&
	                box6x.right == 0x20000 &&
	                box6x.bottom == 0x30000 &&
	                box6x.top == 0x40000 &&
	                box6x.back == 0x50000 &&
	                box6x.front == 0x60000,
	                "Expected {0x10000, 0x20000, 0x30000, 0x40000, 0x50000, 0x60000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
}

static void testIntersection() {
	Rect4d rect4d;
	Rect4f rect4f;
	Rect4i rect4i;
	Rect4x rect4x;
	Box6d box6d;
	Box6f box6f;
	Box6i box6i;
	Box6x box6x;
	
	rect4d = Rect4d_intersection(RECT4d(0.0, 4.0, 1.0, 5.0), RECT4d(2.0, 7.0, 2.0, 6.0));
	TestCase_assert(rect4d.left == 2.0 &&
	                rect4d.right == 4.0 &&
	                rect4d.bottom == 2.0 &&
	                rect4d.top == 5.0,
	                "Expected {2.0, 4.0, 2.0, 5.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_intersection(RECT4d(3.0, 8.0, 3.0, 7.0), RECT4d(1.0, 5.0, 2.0, 6.0));
	TestCase_assert(rect4d.left == 3.0 &&
	                rect4d.right == 5.0 &&
	                rect4d.bottom == 3.0 &&
	                rect4d.top == 6.0,
	                "Expected {3.0, 5.0, 3.0, 6.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_intersection(RECT4d(0.0, 1.0, 0.0, 1.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 0.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_intersection(RECT4d(2.0, 1.0, 2.0, 1.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 0.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_intersection(RECT4d(0.0, 1.0, 0.0, 1.0), RECT4d(2.0, 0.0, 2.0, 0.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 0.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_intersection(RECT4d(2.0, 0.0, 2.0, 0.0), RECT4d(1.0, 0.0, 1.0, 0.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 0.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	
	rect4f = Rect4f_intersection(RECT4f(0.0f, 4.0f, 1.0f, 5.0f), RECT4f(2.0f, 7.0f, 2.0f, 6.0f));
	TestCase_assert(rect4f.left == 2.0f &&
	                rect4f.right == 4.0f &&
	                rect4f.bottom == 2.0f &&
	                rect4f.top == 5.0f,
	                "Expected {2.0, 4.0, 2.0, 5.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_intersection(RECT4f(3.0f, 8.0f, 3.0f, 7.0f), RECT4f(1.0f, 5.0f, 2.0f, 6.0f));
	TestCase_assert(rect4f.left == 3.0f &&
	                rect4f.right == 5.0f &&
	                rect4f.bottom == 3.0f &&
	                rect4f.top == 6.0f,
	                "Expected {3.0, 5.0, 3.0, 6.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_intersection(RECT4f(0.0f, 1.0f, 0.0f, 1.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 0.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_intersection(RECT4f(2.0f, 1.0f, 2.0f, 1.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 0.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_intersection(RECT4f(0.0f, 1.0f, 0.0f, 1.0f), RECT4f(2.0f, 0.0f, 2.0f, 0.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 0.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_intersection(RECT4f(2.0f, 0.0f, 2.0f, 0.0f), RECT4f(1.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 0.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	
	rect4i = Rect4i_intersection(RECT4i(0, 4, 1, 5), RECT4i(2, 7, 2, 6));
	TestCase_assert(rect4i.left == 2 &&
	                rect4i.right == 4 &&
	                rect4i.bottom == 2 &&
	                rect4i.top == 5,
	                "Expected {2, 4, 2, 5} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_intersection(RECT4i(3, 8, 3, 7), RECT4i(1, 5, 2, 6));
	TestCase_assert(rect4i.left == 3 &&
	                rect4i.right == 5 &&
	                rect4i.bottom == 3 &&
	                rect4i.top == 6,
	                "Expected {3, 5, 3, 6} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_intersection(RECT4i(0, 1, 0, 1), RECT4i(1, 2, 1, 2));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 0 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 0,
	                "Expected {0, 0, 0, 0} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_intersection(RECT4i(2, 1, 2, 1), RECT4i(1, 2, 1, 2));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 0 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 0,
	                "Expected {0, 0, 0, 0} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_intersection(RECT4i(0, 1, 0, 1), RECT4i(2, 0, 2, 0));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 0 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 0,
	                "Expected {0, 0, 0, 0} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_intersection(RECT4i(2, 0, 2, 0), RECT4i(1, 0, 1, 0));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 0 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 0,
	                "Expected {0, 0, 0, 0} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	
	rect4x = Rect4x_intersection(RECT4x(0x00000, 0x40000, 0x10000, 0x50000), RECT4x(0x20000, 0x70000, 0x20000, 0x60000));
	TestCase_assert(rect4x.left == 0x20000 &&
	                rect4x.right == 0x40000 &&
	                rect4x.bottom == 0x20000 &&
	                rect4x.top == 0x50000,
	                "Expected {0x20000, 0x40000, 0x20000, 0x50000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_intersection(RECT4x(0x30000, 0x80000, 0x30000, 0x70000), RECT4x(0x10000, 0x50000, 0x20000, 0x60000));
	TestCase_assert(rect4x.left == 0x30000 &&
	                rect4x.right == 0x50000 &&
	                rect4x.bottom == 0x30000 &&
	                rect4x.top == 0x60000,
	                "Expected {0x30000, 0x50000, 0x30000, 0x60000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_intersection(RECT4x(0x00000, 0x10000, 0x00000, 0x10000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x00000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_intersection(RECT4x(0x20000, 0x10000, 0x20000, 0x10000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x00000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_intersection(RECT4x(0x00000, 0x10000, 0x00000, 0x10000), RECT4x(0x20000, 0x00000, 0x20000, 0x00000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x00000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_intersection(RECT4x(0x20000, 0x00000, 0x20000, 0x00000), RECT4x(0x10000, 0x00000, 0x10000, 0x00000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x00000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	
	box6d = Box6d_intersection(BOX6d(0.0, 4.0, 1.0, 5.0, 2.0, 6.0), BOX6d(2.0, 7.0, 2.0, 6.0, 2.0, 5.0));
	TestCase_assert(box6d.left == 2.0 &&
	                box6d.right == 4.0 &&
	                box6d.bottom == 2.0 &&
	                box6d.top == 5.0 &&
	                box6d.back == 2.0 &&
	                box6d.front == 5.0,
	                "Expected {2.0, 4.0, 2.0, 5.0, 2.0, 5.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_intersection(BOX6d(3.0, 8.0, 3.0, 7.0, 3.0, 6.0), BOX6d(1.0, 5.0, 2.0, 6.0, 3.0, 7.0));
	TestCase_assert(box6d.left == 3.0 &&
	                box6d.right == 5.0 &&
	                box6d.bottom == 3.0 &&
	                box6d.top == 6.0 &&
	                box6d.back == 3.0 &&
	                box6d.front == 6.0,
	                "Expected {3.0, 5.0, 3.0, 6.0, 3.0, 6.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_intersection(BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 0.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 0.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_intersection(BOX6d(2.0, 1.0, 2.0, 1.0, 2.0, 1.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 0.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 0.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_intersection(BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0), BOX6d(2.0, 0.0, 2.0, 0.0, 2.0, 0.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 0.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 0.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_intersection(BOX6d(2.0, 0.0, 2.0, 0.0, 2.0, 0.0), BOX6d(1.0, 0.0, 1.0, 0.0, 1.0, 0.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 0.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 0.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	
	box6f = Box6f_intersection(BOX6f(0.0f, 4.0f, 1.0f, 5.0f, 2.0f, 6.0f), BOX6f(2.0f, 7.0f, 2.0f, 6.0f, 2.0f, 5.0f));
	TestCase_assert(box6f.left == 2.0f &&
	                box6f.right == 4.0f &&
	                box6f.bottom == 2.0f &&
	                box6f.top == 5.0f &&
	                box6f.back == 2.0f &&
	                box6f.front == 5.0f,
	                "Expected {2.0, 4.0, 2.0, 5.0, 2.0, 5.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_intersection(BOX6f(3.0f, 8.0f, 3.0f, 7.0f, 3.0f, 6.0f), BOX6f(1.0f, 5.0f, 2.0f, 6.0f, 3.0f, 7.0f));
	TestCase_assert(box6f.left == 3.0f &&
	                box6f.right == 5.0f &&
	                box6f.bottom == 3.0f &&
	                box6f.top == 6.0f &&
	                box6f.back == 3.0f &&
	                box6f.front == 6.0f,
	                "Expected {3.0, 5.0, 3.0, 6.0, 3.0, 6.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_intersection(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 0.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 0.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_intersection(BOX6f(2.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 0.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 0.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_intersection(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f), BOX6f(2.0f, 0.0f, 2.0f, 0.0f, 2.0f, 0.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 0.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 0.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_intersection(BOX6f(2.0f, 0.0f, 2.0f, 0.0f, 2.0f, 0.0f), BOX6f(1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 0.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 0.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	
	box6i = Box6i_intersection(BOX6i(0, 4, 1, 5, 2, 6), BOX6i(2, 7, 2, 6, 2, 5));
	TestCase_assert(box6i.left == 2 &&
	                box6i.right == 4 &&
	                box6i.bottom == 2 &&
	                box6i.top == 5 &&
	                box6i.back == 2 &&
	                box6i.front == 5,
	                "Expected {2, 4, 2, 5, 2, 5} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_intersection(BOX6i(3, 8, 3, 7, 3, 6), BOX6i(1, 5, 2, 6, 3, 7));
	TestCase_assert(box6i.left == 3 &&
	                box6i.right == 5 &&
	                box6i.bottom == 3 &&
	                box6i.top == 6 &&
	                box6i.back == 3 &&
	                box6i.front == 6,
	                "Expected {3, 5, 3, 6, 3, 6} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_intersection(BOX6i(0, 1, 0, 1, 0, 1), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 0 &&
	                box6i.bottom == 0 &&
	                box6i.top == 0 &&
	                box6i.back == 0 &&
	                box6i.front == 0,
	                "Expected {0, 0, 0, 0, 0, 0} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_intersection(BOX6i(2, 1, 2, 1, 2, 1), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 0 &&
	                box6i.bottom == 0 &&
	                box6i.top == 0 &&
	                box6i.back == 0 &&
	                box6i.front == 0,
	                "Expected {0, 0, 0, 0, 0, 0} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_intersection(BOX6i(0, 1, 0, 1, 0, 1), BOX6i(2, 0, 2, 0, 2, 0));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 0 &&
	                box6i.bottom == 0 &&
	                box6i.top == 0 &&
	                box6i.back == 0 &&
	                box6i.front == 0,
	                "Expected {0, 0, 0, 0, 0, 0} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_intersection(BOX6i(2, 0, 2, 0, 2, 0), BOX6i(1, 0, 1, 0, 1, 0));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 0 &&
	                box6i.bottom == 0 &&
	                box6i.top == 0 &&
	                box6i.back == 0 &&
	                box6i.front == 0,
	                "Expected {0, 0, 0, 0, 0, 0} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	
	box6x = Box6x_intersection(BOX6x(0x00000, 0x40000, 0x10000, 0x50000, 0x20000, 0x60000), BOX6x(0x20000, 0x70000, 0x20000, 0x60000, 0x20000, 0x50000));
	TestCase_assert(box6x.left == 0x20000 &&
	                box6x.right == 0x40000 &&
	                box6x.bottom == 0x20000 &&
	                box6x.top == 0x50000 &&
	                box6x.back == 0x20000 &&
	                box6x.front == 0x50000,
	                "Expected {0x20000, 0x40000, 0x20000, 0x50000, 0x20000, 0x50000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_intersection(BOX6x(0x30000, 0x80000, 0x30000, 0x70000, 0x30000, 0x60000), BOX6x(0x10000, 0x50000, 0x20000, 0x60000, 0x30000, 0x70000));
	TestCase_assert(box6x.left == 0x30000 &&
	                box6x.right == 0x50000 &&
	                box6x.bottom == 0x30000 &&
	                box6x.top == 0x60000 &&
	                box6x.back == 0x30000 &&
	                box6x.front == 0x60000,
	                "Expected {0x30000, 0x50000, 0x30000, 0x60000, 0x30000, 0x60000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_intersection(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x00000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x00000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_intersection(BOX6x(0x20000, 0x10000, 0x20000, 0x10000, 0x20000, 0x10000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x00000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x00000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_intersection(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000), BOX6x(0x20000, 0x00000, 0x20000, 0x00000, 0x20000, 0x00000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x00000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x00000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_intersection(BOX6x(0x20000, 0x00000, 0x20000, 0x00000, 0x20000, 0x00000), BOX6x(0x10000, 0x00000, 0x10000, 0x00000, 0x10000, 0x00000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x00000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x00000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
}

static void testUnion() {
	Rect4d rect4d;
	Rect4f rect4f;
	Rect4i rect4i;
	Rect4x rect4x;
	Box6d box6d;
	Box6f box6f;
	Box6i box6i;
	Box6x box6x;
	
	rect4d = Rect4d_union(RECT4d(0.0, 1.0, 0.0, 1.0), RECT4d(2.0, 3.0, 5.0, 7.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 3.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 7.0,
	                "Expected {0.0, 3.0, 0.0, 7.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_union(RECT4d(4.0, 5.0, 2.0, 3.0), RECT4d(0.0, 1.0, 1.0, 2.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 5.0 &&
	                rect4d.bottom == 1.0 &&
	                rect4d.top == 3.0,
	                "Expected {0.0, 5.0, 1.0, 3.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_union(RECT4d(2.0, 3.0, 2.0, 3.0), RECT4d(1.0, 0.0, 1.0, 0.0));
	TestCase_assert(rect4d.left == 2.0 &&
	                rect4d.right == 3.0 &&
	                rect4d.bottom == 2.0 &&
	                rect4d.top == 3.0,
	                "Expected {2.0, 3.0, 2.0, 3.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_union(RECT4d(3.0, 2.0, 3.0, 2.0), RECT4d(0.0, 1.0, 0.0, 1.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 1.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 1.0,
	                "Expected {0.0, 1.0, 0.0, 1.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_union(RECT4d(3.0, 2.0, 3.0, 2.0), RECT4d(1.0, 0.0, 1.0, 0.0));
	TestCase_assert(rect4d.left == 0.0 &&
	                rect4d.right == 0.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	
	rect4f = Rect4f_union(RECT4f(0.0f, 1.0f, 0.0f, 1.0f), RECT4f(2.0f, 3.0f, 5.0f, 7.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 3.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 7.0f,
	                "Expected {0.0, 3.0, 0.0, 7.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_union(RECT4f(4.0f, 5.0f, 2.0f, 3.0f), RECT4f(0.0f, 1.0f, 1.0f, 2.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 5.0f &&
	                rect4f.bottom == 1.0f &&
	                rect4f.top == 3.0f,
	                "Expected {0.0, 5.0, 1.0, 3.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_union(RECT4f(2.0f, 3.0f, 2.0f, 3.0f), RECT4f(1.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(rect4f.left == 2.0f &&
	                rect4f.right == 3.0f &&
	                rect4f.bottom == 2.0f &&
	                rect4f.top == 3.0f,
	                "Expected {2.0, 3.0, 2.0, 3.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_union(RECT4f(3.0f, 2.0f, 3.0f, 2.0f), RECT4f(0.0f, 1.0f, 0.0f, 1.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 1.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 1.0f,
	                "Expected {0.0, 1.0, 0.0, 1.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_union(RECT4f(3.0f, 2.0f, 3.0f, 2.0f), RECT4f(1.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(rect4f.left == 0.0f &&
	                rect4f.right == 0.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	
	rect4i = Rect4i_union(RECT4i(0, 1, 0, 1), RECT4i(2, 3, 5, 7));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 3 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 7,
	                "Expected {0, 3, 0, 7} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_union(RECT4i(4, 5, 2, 3), RECT4i(0, 1, 1, 2));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 5 &&
	                rect4i.bottom == 1 &&
	                rect4i.top == 3,
	                "Expected {0, 5, 1, 3} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_union(RECT4i(2, 3, 2, 3), RECT4i(1, 0, 1, 0));
	TestCase_assert(rect4i.left == 2 &&
	                rect4i.right == 3 &&
	                rect4i.bottom == 2 &&
	                rect4i.top == 3,
	                "Expected {2, 3, 2, 3} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_union(RECT4i(3, 2, 3, 2), RECT4i(0, 1, 0, 1));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 1 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 1,
	                "Expected {0, 1, 0, 1} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_union(RECT4i(3, 2, 3, 2), RECT4i(1, 0, 1, 0));
	TestCase_assert(rect4i.left == 0 &&
	                rect4i.right == 0 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 0,
	                "Expected {0, 0, 0, 0} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	
	rect4x = Rect4x_union(RECT4x(0x00000, 0x10000, 0x00000, 0x10000), RECT4x(0x20000, 0x30000, 0x50000, 0x70000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x30000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x70000,
	                "Expected {0x00000, 0x30000, 0x00000, 0x70000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_union(RECT4x(0x40000, 0x50000, 0x20000, 0x30000), RECT4x(0x00000, 0x10000, 0x10000, 0x20000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x50000 &&
	                rect4x.bottom == 0x10000 &&
	                rect4x.top == 0x30000,
	                "Expected {0x00000, 0x50000, 0x10000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_union(RECT4x(0x20000, 0x30000, 0x20000, 0x30000), RECT4x(0x10000, 0x00000, 0x10000, 0x00000));
	TestCase_assert(rect4x.left == 0x20000 &&
	                rect4x.right == 0x30000 &&
	                rect4x.bottom == 0x20000 &&
	                rect4x.top == 0x30000,
	                "Expected {0x20000, 0x30000, 0x20000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_union(RECT4x(0x30000, 0x20000, 0x30000, 0x20000), RECT4x(0x00000, 0x10000, 0x00000, 0x10000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x10000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x10000,
	                "Expected {0x00000, 0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_union(RECT4x(0x30000, 0x20000, 0x30000, 0x20000), RECT4x(0x10000, 0x00000, 0x10000, 0x00000));
	TestCase_assert(rect4x.left == 0x00000 &&
	                rect4x.right == 0x00000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	
	box6d = Box6d_union(BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0), BOX6d(2.0, 3.0, 5.0, 7.0, 6.0, 8.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 3.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 7.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 8.0,
	                "Expected {0.0, 3.0, 0.0, 7.0, 0.0, 8.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_union(BOX6d(4.0, 5.0, 2.0, 3.0, 1.0, 2.0), BOX6d(0.0, 1.0, 1.0, 2.0, 2.0, 3.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 5.0 &&
	                box6d.bottom == 1.0 &&
	                box6d.top == 3.0 &&
	                box6d.back == 1.0 &&
	                box6d.front == 3.0,
	                "Expected {0.0, 5.0, 1.0, 3.0, 1.0, 3.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_union(BOX6d(2.0, 3.0, 2.0, 3.0, 2.0, 3.0), BOX6d(1.0, 0.0, 1.0, 0.0, 1.0, 0.0));
	TestCase_assert(box6d.left == 2.0 &&
	                box6d.right == 3.0 &&
	                box6d.bottom == 2.0 &&
	                box6d.top == 3.0 &&
	                box6d.back == 2.0 &&
	                box6d.front == 3.0,
	                "Expected {2.0, 3.0, 2.0, 3.0, 2.0, 3.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_union(BOX6d(3.0, 2.0, 3.0, 2.0, 3.0, 2.0), BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 1.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 1.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 1.0,
	                "Expected {0.0, 1.0, 0.0, 1.0, 0.0, 1.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_union(BOX6d(3.0, 2.0, 3.0, 2.0, 3.0, 2.0), BOX6d(1.0, 0.0, 1.0, 0.0, 1.0, 0.0));
	TestCase_assert(box6d.left == 0.0 &&
	                box6d.right == 0.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 0.0 &&
	                box6d.back == 0.0 &&
	                box6d.front == 0.0,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	
	box6f = Box6f_union(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f), BOX6f(2.0f, 3.0f, 5.0f, 7.0f, 6.0f, 8.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 3.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 7.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 8.0f,
	                "Expected {0.0, 3.0, 0.0, 7.0, 0.0, 8.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_union(BOX6f(4.0f, 5.0f, 2.0f, 3.0f, 1.0f, 2.0f), BOX6f(0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 3.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 5.0f &&
	                box6f.bottom == 1.0f &&
	                box6f.top == 3.0f &&
	                box6f.back == 1.0f &&
	                box6f.front == 3.0f,
	                "Expected {0.0, 5.0, 1.0, 3.0, 1.0, 3.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_union(BOX6f(2.0f, 3.0f, 2.0f, 3.0f, 2.0f, 3.0f), BOX6f(1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(box6f.left == 2.0f &&
	                box6f.right == 3.0f &&
	                box6f.bottom == 2.0f &&
	                box6f.top == 3.0f &&
	                box6f.back == 2.0f &&
	                box6f.front == 3.0f,
	                "Expected {2.0, 3.0, 2.0, 3.0, 2.0, 3.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_union(BOX6f(3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f), BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 1.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 1.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 1.0f,
	                "Expected {0.0, 1.0, 0.0, 1.0, 0.0, 1.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_union(BOX6f(3.0f, 2.0f, 3.0f, 2.0f, 3.0f, 2.0f), BOX6f(1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	TestCase_assert(box6f.left == 0.0f &&
	                box6f.right == 0.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 0.0f &&
	                box6f.back == 0.0f &&
	                box6f.front == 0.0f,
	                "Expected {0.0, 0.0, 0.0, 0.0, 0.0, 0.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	
	box6i = Box6i_union(BOX6i(0, 1, 0, 1, 0, 1), BOX6i(2, 3, 5, 7, 6, 8));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 3 &&
	                box6i.bottom == 0 &&
	                box6i.top == 7 &&
	                box6i.back == 0 &&
	                box6i.front == 8,
	                "Expected {0, 3, 0, 7, 0, 8} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_union(BOX6i(4, 5, 2, 3, 1, 2), BOX6i(0, 1, 1, 2, 2, 3));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 5 &&
	                box6i.bottom == 1 &&
	                box6i.top == 3 &&
	                box6i.back == 1 &&
	                box6i.front == 3,
	                "Expected {0, 5, 1, 3, 1, 3} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_union(BOX6i(2, 3, 2, 3, 2, 3), BOX6i(1, 0, 1, 0, 1, 0));
	TestCase_assert(box6i.left == 2 &&
	                box6i.right == 3 &&
	                box6i.bottom == 2 &&
	                box6i.top == 3 &&
	                box6i.back == 2 &&
	                box6i.front == 3,
	                "Expected {2, 3, 2, 3, 2, 3} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_union(BOX6i(3, 2, 3, 2, 3, 2), BOX6i(0, 1, 0, 1, 0, 1));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 1 &&
	                box6i.bottom == 0 &&
	                box6i.top == 1 &&
	                box6i.back == 0 &&
	                box6i.front == 1,
	                "Expected {0, 1, 0, 1, 0, 1} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_union(BOX6i(3, 2, 3, 2, 3, 2), BOX6i(1, 0, 1, 0, 1, 0));
	TestCase_assert(box6i.left == 0 &&
	                box6i.right == 0 &&
	                box6i.bottom == 0 &&
	                box6i.top == 0 &&
	                box6i.back == 0 &&
	                box6i.front == 0,
	                "Expected {0, 0, 0, 0, 0, 0} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	
	box6x = Box6x_union(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000), BOX6x(0x20000, 0x30000, 0x50000, 0x70000, 0x60000, 0x80000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x30000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x70000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x80000,
	                "Expected {0x00000, 0x30000, 0x00000, 0x70000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_union(BOX6x(0x40000, 0x50000, 0x20000, 0x30000, 0x10000, 0x20000), BOX6x(0x00000, 0x10000, 0x10000, 0x20000, 0x20000, 0x30000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x50000 &&
	                box6x.bottom == 0x10000 &&
	                box6x.top == 0x30000 &&
	                box6x.back == 0x10000 &&
	                box6x.front == 0x30000,
	                "Expected {0x00000, 0x50000, 0x10000, 0x30000, 0x10000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_union(BOX6x(0x20000, 0x30000, 0x20000, 0x30000, 0x20000, 0x30000), BOX6x(0x10000, 0x00000, 0x10000, 0x00000, 0x10000, 0x00000));
	TestCase_assert(box6x.left == 0x20000 &&
	                box6x.right == 0x30000 &&
	                box6x.bottom == 0x20000 &&
	                box6x.top == 0x30000 &&
	                box6x.back == 0x20000 &&
	                box6x.front == 0x30000,
	                "Expected {0x20000, 0x30000, 0x20000, 0x30000, 0x20000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_union(BOX6x(0x30000, 0x20000, 0x30000, 0x20000, 0x30000, 0x20000), BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x10000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x10000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x10000,
	                "Expected {0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_union(BOX6x(0x30000, 0x20000, 0x30000, 0x20000, 0x30000, 0x20000), BOX6x(0x10000, 0x00000, 0x10000, 0x00000, 0x10000, 0x00000));
	TestCase_assert(box6x.left == 0x00000 &&
	                box6x.right == 0x00000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x00000 &&
	                box6x.back == 0x00000 &&
	                box6x.front == 0x00000,
	                "Expected {0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
}

static void testOffset() {
	Rect4d rect4d;
	Rect4f rect4f;
	Rect4i rect4i;
	Rect4x rect4x;
	Box6d box6d;
	Box6f box6f;
	Box6i box6i;
	Box6x box6x;
	
	rect4d = Rect4d_offset(RECT4d(0.0, 1.0, 0.0, 1.0), VECTOR2d(1.0, 2.0));
	TestCase_assert(rect4d.left == 1.0 &&
	                rect4d.right == 2.0 &&
	                rect4d.bottom == 2.0 &&
	                rect4d.top == 3.0,
	                "Expected {1.0, 2.0, 2.0, 3.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_offset(RECT4d(3.0, 5.0, 1.0, 2.0), VECTOR2d(2.0, -1.0));
	TestCase_assert(rect4d.left == 4.0 &&
	                rect4d.right == 6.0 &&
	                rect4d.bottom == 0.0 &&
	                rect4d.top == 1.0,
	                "Expected {4.0, 6.0, 0.0, 1.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	
	rect4f = Rect4f_offset(RECT4f(0.0f, 1.0f, 0.0f, 1.0f), VECTOR2f(1.0f, 2.0f));
	TestCase_assert(rect4f.left == 1.0f &&
	                rect4f.right == 2.0f &&
	                rect4f.bottom == 2.0f &&
	                rect4f.top == 3.0f,
	                "Expected {1.0, 2.0, 2.0, 3.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_offset(RECT4f(3.0f, 5.0f, 1.0f, 2.0f), VECTOR2f(2.0f, -1.0f));
	TestCase_assert(rect4f.left == 4.0f &&
	                rect4f.right == 6.0f &&
	                rect4f.bottom == 0.0f &&
	                rect4f.top == 1.0f,
	                "Expected {4.0, 6.0, 0.0, 1.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	
	rect4i = Rect4i_offset(RECT4i(0, 1, 0, 1), VECTOR2i(1, 2));
	TestCase_assert(rect4i.left == 1 &&
	                rect4i.right == 2 &&
	                rect4i.bottom == 2 &&
	                rect4i.top == 3,
	                "Expected {1, 2, 2, 3} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_offset(RECT4i(3, 5, 1, 2), VECTOR2i(2, -1));
	TestCase_assert(rect4i.left == 4 &&
	                rect4i.right == 6 &&
	                rect4i.bottom == 0 &&
	                rect4i.top == 1,
	                "Expected {4, 6, 0, 1} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	
	rect4x = Rect4x_offset(RECT4x(0x00000, 0x10000, 0x00000, 0x10000), VECTOR2x(0x10000, 0x20000));
	TestCase_assert(rect4x.left == 0x10000 &&
	                rect4x.right == 0x20000 &&
	                rect4x.bottom == 0x20000 &&
	                rect4x.top == 0x30000,
	                "Expected {0x10000, 0x20000, 0x20000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_offset(RECT4x(0x30000, 0x50000, 0x10000, 0x20000), VECTOR2x(0x20000, -0x10000));
	TestCase_assert(rect4x.left == 0x40000 &&
	                rect4x.right == 0x60000 &&
	                rect4x.bottom == 0x00000 &&
	                rect4x.top == 0x10000,
	                "Expected {0x40000, 0x60000, 0x00000, 0x10000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	
	box6d = Box6d_offset(BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0), VECTOR3d(1.0, 2.0, 3.0));
	TestCase_assert(box6d.left == 1.0 &&
	                box6d.right == 2.0 &&
	                box6d.bottom == 2.0 &&
	                box6d.top == 3.0 &&
	                box6d.back == 3.0 &&
	                box6d.front == 4.0,
	                "Expected {1.0, 2.0, 2.0, 3.0, 3.0, 4.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_offset(BOX6d(3.0, 5.0, 1.0, 2.0, 6.0, 7.0), VECTOR3d(2.0, -1.0, 1.0));
	TestCase_assert(box6d.left == 4.0 &&
	                box6d.right == 6.0 &&
	                box6d.bottom == 0.0 &&
	                box6d.top == 1.0 &&
	                box6d.back == 7.0 &&
	                box6d.front == 8.0,
	                "Expected {4.0, 6.0, 0.0, 1.0, 7.0, 8.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	
	box6f = Box6f_offset(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f), VECTOR3f(1.0f, 2.0f, 3.0f));
	TestCase_assert(box6f.left == 1.0f &&
	                box6f.right == 2.0f &&
	                box6f.bottom == 2.0f &&
	                box6f.top == 3.0f &&
	                box6f.back == 3.0f &&
	                box6f.front == 4.0f,
	                "Expected {1.0, 2.0, 2.0, 3.0, 3.0, 4.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_offset(BOX6f(3.0f, 5.0f, 1.0f, 2.0f, 6.0f, 7.0f), VECTOR3f(2.0f, -1.0f, 1.0f));
	TestCase_assert(box6f.left == 4.0f &&
	                box6f.right == 6.0f &&
	                box6f.bottom == 0.0f &&
	                box6f.top == 1.0f &&
	                box6f.back == 7.0f &&
	                box6f.front == 8.0f,
	                "Expected {4.0, 6.0, 0.0, 1.0, 7.0, 8.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	
	box6i = Box6i_offset(BOX6i(0, 1, 0, 1, 0, 1), VECTOR3i(1, 2, 3));
	TestCase_assert(box6i.left == 1 &&
	                box6i.right == 2 &&
	                box6i.bottom == 2 &&
	                box6i.top == 3 &&
	                box6i.back == 3 &&
	                box6i.front == 4,
	                "Expected {1, 2, 2, 3, 3, 4} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_offset(BOX6i(3, 5, 1, 2, 6, 7), VECTOR3i(2, -1, 1));
	TestCase_assert(box6i.left == 4 &&
	                box6i.right == 6 &&
	                box6i.bottom == 0 &&
	                box6i.top == 1 &&
	                box6i.back == 7 &&
	                box6i.front == 8,
	                "Expected {4, 6, 0, 1, 7, 8} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	
	box6x = Box6x_offset(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000), VECTOR3x(0x10000, 0x20000, 0x30000));
	TestCase_assert(box6x.left == 0x10000 &&
	                box6x.right == 0x20000 &&
	                box6x.bottom == 0x20000 &&
	                box6x.top == 0x30000 &&
	                box6x.bottom == 0x30000 &&
	                box6x.top == 0x40000,
	                "Expected {0x10000, 0x20000, 0x20000, 0x30000, 0x30000, 0x40000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_offset(BOX6x(0x30000, 0x50000, 0x10000, 0x20000, 0x60000, 0x70000), VECTOR3x(0x20000, -0x10000, 0x10000));
	TestCase_assert(box6x.left == 0x40000 &&
	                box6x.right == 0x60000 &&
	                box6x.bottom == 0x00000 &&
	                box6x.top == 0x10000 &&
	                box6x.back == 0x70000 &&
	                box6x.front == 0x80000,
	                "Expected {0x40000, 0x60000, 0x00000, 0x10000, 0x70000, 0x80000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
}

static void testInset() {
	Rect4d rect4d;
	Rect4f rect4f;
	Rect4i rect4i;
	Rect4x rect4x;
	Box6d box6d;
	Box6f box6f;
	Box6i box6i;
	Box6x box6x;
	
	rect4d = Rect4d_inset(RECT4d(0.0, 6.0, 0.0, 6.0), VECTOR2d(1.0, 2.0));
	TestCase_assert(rect4d.left == 1.0 &&
	                rect4d.right == 5.0 &&
	                rect4d.bottom == 2.0 &&
	                rect4d.top == 4.0,
	                "Expected {1.0, 5.0, 2.0, 4.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	rect4d = Rect4d_inset(RECT4d(1.0, 10.0, 2.0, 9.0), VECTOR2d(2.0, -1.0));
	TestCase_assert(rect4d.left == 3.0 &&
	                rect4d.right == 8.0 &&
	                rect4d.bottom == 1.0 &&
	                rect4d.top == 10.0,
	                "Expected {3.0, 8.0, 1.0, 10.0} but got {%f, %f, %f, %f}",
	                rect4d.left,
	                rect4d.right,
	                rect4d.bottom,
	                rect4d.top);
	
	rect4f = Rect4f_inset(RECT4f(0.0f, 6.0f, 0.0f, 6.0f), VECTOR2f(1.0f, 2.0f));
	TestCase_assert(rect4f.left == 1.0f &&
	                rect4f.right == 5.0f &&
	                rect4f.bottom == 2.0f &&
	                rect4f.top == 4.0f,
	                "Expected {1.0, 5.0, 2.0, 4.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	rect4f = Rect4f_inset(RECT4f(1.0f, 10.0f, 2.0f, 9.0f), VECTOR2f(2.0f, -1.0f));
	TestCase_assert(rect4f.left == 3.0f &&
	                rect4f.right == 8.0f &&
	                rect4f.bottom == 1.0f &&
	                rect4f.top == 10.0f,
	                "Expected {3.0, 8.0, 1.0, 10.0} but got {%f, %f, %f, %f}",
	                rect4f.left,
	                rect4f.right,
	                rect4f.bottom,
	                rect4f.top);
	
	rect4i = Rect4i_inset(RECT4i(0, 6, 0, 6), VECTOR2i(1, 2));
	TestCase_assert(rect4i.left == 1 &&
	                rect4i.right == 5 &&
	                rect4i.bottom == 2 &&
	                rect4i.top == 4,
	                "Expected {1, 5, 2, 4} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	rect4i = Rect4i_inset(RECT4i(1, 10, 2, 9), VECTOR2i(2, -1));
	TestCase_assert(rect4i.left == 3 &&
	                rect4i.right == 8 &&
	                rect4i.bottom == 1 &&
	                rect4i.top == 10,
	                "Expected {3, 8, 1, 10} but got {%d, %d, %d, %d}",
	                rect4i.left,
	                rect4i.right,
	                rect4i.bottom,
	                rect4i.top);
	
	rect4x = Rect4x_inset(RECT4x(0x00000, 0x60000, 0x00000, 0x60000), VECTOR2x(0x10000, 0x20000));
	TestCase_assert(rect4x.left == 0x10000 &&
	                rect4x.right == 0x50000 &&
	                rect4x.bottom == 0x20000 &&
	                rect4x.top == 0x40000,
	                "Expected {0x10000, 0x50000, 0x20000, 0x40000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	rect4x = Rect4x_inset(RECT4x(0x10000, 0xA0000, 0x20000, 0x90000), VECTOR2x(0x20000, -0x10000));
	TestCase_assert(rect4x.left == 0x30000 &&
	                rect4x.right == 0x80000 &&
	                rect4x.bottom == 0x10000 &&
	                rect4x.top == 0xA0000,
	                "Expected {0x30000, 0x80000, 0x10000, 0xA0000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                rect4x.left,
	                rect4x.right,
	                rect4x.bottom,
	                rect4x.top);
	
	box6d = Box6d_inset(BOX6d(0.0, 6.0, 0.0, 6.0, 0.0, 6.0), VECTOR3d(1.0, 2.0, 3.0));
	TestCase_assert(box6d.left == 1.0 &&
	                box6d.right == 5.0 &&
	                box6d.bottom == 2.0 &&
	                box6d.top == 4.0 &&
	                box6d.back == 3.0 &&
	                box6d.front == 3.0,
	                "Expected {1.0, 5.0, 2.0, 4.0, 3.0, 3.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	box6d = Box6d_inset(BOX6d(1.0, 10.0, 2.0, 9.0, 3.0, 8.0), VECTOR3d(2.0, -1.0, 1.0));
	TestCase_assert(box6d.left == 3.0 &&
	                box6d.right == 8.0 &&
	                box6d.bottom == 1.0 &&
	                box6d.top == 10.0 &&
	                box6d.back == 4.0 &&
	                box6d.front == 7.0,
	                "Expected {3.0, 8.0, 1.0, 10.0, 4.0, 7.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6d.left,
	                box6d.right,
	                box6d.bottom,
	                box6d.top,
	                box6d.back,
	                box6d.front);
	
	box6f = Box6f_inset(BOX6f(0.0f, 6.0f, 0.0f, 6.0f, 0.0f, 6.0f), VECTOR3f(1.0f, 2.0f, 3.0f));
	TestCase_assert(box6f.left == 1.0f &&
	                box6f.right == 5.0f &&
	                box6f.bottom == 2.0f &&
	                box6f.top == 4.0f &&
	                box6f.back == 3.0f &&
	                box6f.front == 3.0f,
	                "Expected {1.0, 5.0, 2.0, 4.0, 3.0, 3.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	box6f = Box6f_inset(BOX6f(1.0f, 10.0f, 2.0f, 9.0f, 3.0f, 8.0f), VECTOR3f(2.0f, -1.0f, 1.0f));
	TestCase_assert(box6f.left == 3.0f &&
	                box6f.right == 8.0f &&
	                box6f.bottom == 1.0f &&
	                box6f.top == 10.0f &&
	                box6f.back == 4.0f &&
	                box6f.front == 7.0f,
	                "Expected {3.0, 8.0, 1.0, 10.0, 4.0, 7.0} but got {%f, %f, %f, %f, %f, %f}",
	                box6f.left,
	                box6f.right,
	                box6f.bottom,
	                box6f.top,
	                box6f.back,
	                box6f.front);
	
	box6i = Box6i_inset(BOX6i(0, 6, 0, 6, 0, 6), VECTOR3i(1, 2, 3));
	TestCase_assert(box6i.left == 1 &&
	                box6i.right == 5 &&
	                box6i.bottom == 2 &&
	                box6i.top == 4 &&
	                box6i.back == 3 &&
	                box6i.front == 3,
	                "Expected {1, 5, 2, 4, 3, 3} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	box6i = Box6i_inset(BOX6i(1, 10, 2, 9, 3, 8), VECTOR3i(2, -1, 1));
	TestCase_assert(box6i.left == 3 &&
	                box6i.right == 8 &&
	                box6i.bottom == 1 &&
	                box6i.top == 10 &&
	                box6i.back == 4 &&
	                box6i.front == 7,
	                "Expected {3, 8, 1, 10, 4, 7} but got {%d, %d, %d, %d, %d, %d}",
	                box6i.left,
	                box6i.right,
	                box6i.bottom,
	                box6i.top,
	                box6i.back,
	                box6i.front);
	
	box6x = Box6x_inset(BOX6x(0x00000, 0x60000, 0x00000, 0x60000, 0x00000, 0x60000), VECTOR3x(0x10000, 0x20000, 0x30000));
	TestCase_assert(box6x.left == 0x10000 &&
	                box6x.right == 0x50000 &&
	                box6x.bottom == 0x20000 &&
	                box6x.top == 0x40000 &&
	                box6x.bottom == 0x30000 &&
	                box6x.top == 0x30000,
	                "Expected {0x10000, 0x50000, 0x20000, 0x40000, 0x30000, 0x30000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
	box6x = Box6x_inset(BOX6x(0x10000, 0xA0000, 0x20000, 0x90000, 0x30000, 0x80000), VECTOR3x(0x20000, -0x10000, 0x10000));
	TestCase_assert(box6x.left == 0x30000 &&
	                box6x.right == 0x80000 &&
	                box6x.bottom == 0x10000 &&
	                box6x.top == 0xA0000 &&
	                box6x.back == 0x40000 &&
	                box6x.front == 0x70000,
	                "Expected {0x30000, 0x80000, 0x10000, 0xA0000, 0x40000, 0x70000} but got {0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X, 0x%05X}",
	                box6x.left,
	                box6x.right,
	                box6x.bottom,
	                box6x.top,
	                box6x.back,
	                box6x.front);
}

static void testIsEmpty() {
	bool empty;
	
	empty = Rect4d_isEmpty(RECT4d(0.0, 1.0, 0.0, 1.0));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Rect4d_isEmpty(RECT4d(0.0, 0.0, 0.0, 0.0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4d_isEmpty(RECT4d(0.0, 1.0, 1.0, 0.0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4d_isEmpty(RECT4d(1.0, 0.0, 0.0, 1.0));
	TestCase_assert(empty, "Expected true but got false");
	
	empty = Rect4f_isEmpty(RECT4f(0.0f, 1.0f, 0.0f, 1.0f));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Rect4f_isEmpty(RECT4f(0.0f, 0.0f, 0.0f, 0.0f));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4f_isEmpty(RECT4f(0.0f, 1.0f, 1.0f, 0.0f));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4f_isEmpty(RECT4f(1.0f, 0.0f, 0.0f, 1.0f));
	TestCase_assert(empty, "Expected true but got false");
	
	empty = Rect4i_isEmpty(RECT4i(0, 1, 0, 1));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Rect4i_isEmpty(RECT4i(0, 0, 0, 0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4i_isEmpty(RECT4i(0, 1, 1, 0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4i_isEmpty(RECT4i(1, 0, 0, 1));
	TestCase_assert(empty, "Expected true but got false");
	
	empty = Rect4x_isEmpty(RECT4x(0x00000, 0x10000, 0x00000, 0x10000));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Rect4x_isEmpty(RECT4x(0x00000, 0x00000, 0x00000, 0x00000));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4x_isEmpty(RECT4x(0x00000, 0x10000, 0x10000, 0x00000));
	TestCase_assert(empty, "Expected true but got false");
	empty = Rect4x_isEmpty(RECT4x(0x10000, 0x00000, 0x00000, 0x10000));
	TestCase_assert(empty, "Expected true but got false");
	
	empty = Box6d_isEmpty(BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Box6d_isEmpty(BOX6d(0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6d_isEmpty(BOX6d(0.0, 1.0, 0.0, 1.0, 1.0, 0.0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6d_isEmpty(BOX6d(0.0, 1.0, 1.0, 0.0, 0.0, 1.0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6d_isEmpty(BOX6d(1.0, 0.0, 0.0, 1.0, 0.0, 1.0));
	TestCase_assert(empty, "Expected true but got false");
	
	empty = Box6f_isEmpty(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Box6f_isEmpty(BOX6f(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6f_isEmpty(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6f_isEmpty(BOX6f(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6f_isEmpty(BOX6f(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	TestCase_assert(empty, "Expected true but got false");
	
	empty = Box6i_isEmpty(BOX6i(0, 1, 0, 1, 0, 1));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Box6i_isEmpty(BOX6i(0, 0, 0, 0, 0, 0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6i_isEmpty(BOX6i(0, 1, 0, 1, 1, 0));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6i_isEmpty(BOX6i(0, 1, 1, 0, 0, 1));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6i_isEmpty(BOX6i(1, 0, 0, 1, 0, 1));
	TestCase_assert(empty, "Expected true but got false");
	
	empty = Box6x_isEmpty(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000));
	TestCase_assert(!empty, "Expected false but got true");
	empty = Box6x_isEmpty(BOX6x(0x00000, 0x00000, 0x00000, 0x00000, 0x00000, 0x00000));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6x_isEmpty(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x10000, 0x00000));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6x_isEmpty(BOX6x(0x00000, 0x10000, 0x10000, 0x00000, 0x00000, 0x10000));
	TestCase_assert(empty, "Expected true but got false");
	empty = Box6x_isEmpty(BOX6x(0x10000, 0x00000, 0x00000, 0x10000, 0x00000, 0x10000));
	TestCase_assert(empty, "Expected true but got false");
}

static void testIntersectsRectBox() {
	bool intersects;
	
	intersects = Rect4d_intersectsRect4d(RECT4d(0.0, 3.0, 0.0, 3.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4d_intersectsRect4d(RECT4d(0.0, 2.0, 0.0, 2.0), RECT4d(1.0, 3.0, 1.0, 3.0));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4d_intersectsRect4d(RECT4d(0.0, 1.0, 0.0, 1.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4d_intersectsRect4d(RECT4d(0.0, 3.0, 0.0, 3.0), RECT4d(2.0, 1.0, 2.0, 1.0));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4d_intersectsRect4d(RECT4d(3.0, 0.0, 3.0, 0.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!intersects, "Expected false but got true");
	
	intersects = Rect4f_intersectsRect4f(RECT4f(0.0f, 3.0f, 0.0f, 3.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4f_intersectsRect4f(RECT4f(0.0f, 2.0f, 0.0f, 2.0f), RECT4f(1.0f, 3.0f, 1.0f, 3.0f));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4f_intersectsRect4f(RECT4f(0.0f, 1.0f, 0.0f, 1.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4f_intersectsRect4f(RECT4f(0.0f, 3.0f, 0.0f, 3.0f), RECT4f(2.0f, 1.0f, 2.0f, 1.0f));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4f_intersectsRect4f(RECT4f(3.0f, 0.0f, 3.0f, 0.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!intersects, "Expected false but got true");
	
	intersects = Rect4i_intersectsRect4i(RECT4i(0, 3, 0, 3), RECT4i(1, 2, 1, 2));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4i_intersectsRect4i(RECT4i(0, 2, 0, 2), RECT4i(1, 3, 1, 3));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4i_intersectsRect4i(RECT4i(0, 1, 0, 1), RECT4i(1, 2, 1, 2));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4i_intersectsRect4i(RECT4i(0, 3, 0, 3), RECT4i(2, 1, 2, 1));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4i_intersectsRect4i(RECT4i(3, 0, 3, 0), RECT4i(1, 2, 1, 2));
	TestCase_assert(!intersects, "Expected false but got true");
	
	intersects = Rect4x_intersectsRect4x(RECT4x(0x00000, 0x30000, 0x00000, 0x30000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4x_intersectsRect4x(RECT4x(0x00000, 0x20000, 0x00000, 0x20000), RECT4x(0x10000, 0x30000, 0x10000, 0x30000));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Rect4x_intersectsRect4x(RECT4x(0x00000, 0x10000, 0x00000, 0x10000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4x_intersectsRect4x(RECT4x(0x00000, 0x30000, 0x00000, 0x30000), RECT4x(0x20000, 0x10000, 0x20000, 0x10000));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Rect4x_intersectsRect4x(RECT4x(0x30000, 0x00000, 0x30000, 0x00000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!intersects, "Expected false but got true");
	
	intersects = Box6d_intersectsBox6d(BOX6d(0.0, 3.0, 0.0, 3.0, 0.0, 3.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6d_intersectsBox6d(BOX6d(0.0, 2.0, 0.0, 2.0, 0.0, 2.0), BOX6d(1.0, 3.0, 1.0, 3.0, 1.0, 3.0));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6d_intersectsBox6d(BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6d_intersectsBox6d(BOX6d(0.0, 3.0, 0.0, 3.0, 0.0, 3.0), BOX6d(2.0, 1.0, 2.0, 1.0, 2.0, 1.0));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6d_intersectsBox6d(BOX6d(3.0, 0.0, 3.0, 0.0, 3.0, 0.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!intersects, "Expected false but got true");
	
	intersects = Box6f_intersectsBox6f(BOX6f(0.0f, 3.0f, 0.0f, 3.0f, 0.0f, 3.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6f_intersectsBox6f(BOX6f(0.0f, 2.0f, 0.0f, 2.0f, 0.0f, 2.0f), BOX6f(1.0f, 3.0f, 1.0f, 3.0f, 1.0f, 3.0f));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6f_intersectsBox6f(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6f_intersectsBox6f(BOX6f(0.0f, 3.0f, 0.0f, 3.0f, 0.0f, 3.0f), BOX6f(2.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6f_intersectsBox6f(BOX6f(3.0f, 0.0f, 3.0f, 0.0f, 3.0f, 0.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!intersects, "Expected false but got true");
	
	intersects = Box6i_intersectsBox6i(BOX6i(0, 3, 0, 3, 0, 3), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6i_intersectsBox6i(BOX6i(0, 2, 0, 2, 0, 2), BOX6i(1, 3, 1, 3, 1, 3));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6i_intersectsBox6i(BOX6i(0, 1, 0, 1, 0, 1), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6i_intersectsBox6i(BOX6i(0, 3, 0, 3, 0, 3), BOX6i(2, 1, 2, 1, 2, 1));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6i_intersectsBox6i(BOX6i(3, 0, 3, 0, 3, 0), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(!intersects, "Expected false but got true");
	
	intersects = Box6x_intersectsBox6x(BOX6x(0x00000, 0x30000, 0x00000, 0x30000, 0x00000, 0x30000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6x_intersectsBox6x(BOX6x(0x00000, 0x20000, 0x00000, 0x20000, 0x00000, 0x20000), BOX6x(0x10000, 0x30000, 0x10000, 0x30000, 0x10000, 0x30000));
	TestCase_assert(intersects, "Expected true but got false");
	intersects = Box6x_intersectsBox6x(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6x_intersectsBox6x(BOX6x(0x00000, 0x30000, 0x00000, 0x30000, 0x00000, 0x30000), BOX6x(0x20000, 0x10000, 0x20000, 0x10000, 0x20000, 0x10000));
	TestCase_assert(!intersects, "Expected false but got true");
	intersects = Box6x_intersectsBox6x(BOX6x(0x30000, 0x00000, 0x30000, 0x00000, 0x30000, 0x00000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!intersects, "Expected false but got true");
}

static void testContainsRectBox() {
	bool contains;
	
	contains = Rect4d_containsRect4d(RECT4d(0.0, 3.0, 0.0, 3.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4d_containsRect4d(RECT4d(0.0, 2.0, 0.0, 2.0), RECT4d(1.0, 3.0, 1.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4d_containsRect4d(RECT4d(0.0, 1.0, 0.0, 1.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4d_containsRect4d(RECT4d(0.0, 3.0, 0.0, 3.0), RECT4d(2.0, 1.0, 2.0, 1.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4d_containsRect4d(RECT4d(3.0, 0.0, 3.0, 0.0), RECT4d(1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!contains, "Expected false but got true");
	
	contains = Rect4f_containsRect4f(RECT4f(0.0f, 3.0f, 0.0f, 3.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4f_containsRect4f(RECT4f(0.0f, 2.0f, 0.0f, 2.0f), RECT4f(1.0f, 3.0f, 1.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4f_containsRect4f(RECT4f(0.0f, 1.0f, 0.0f, 1.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4f_containsRect4f(RECT4f(0.0f, 3.0f, 0.0f, 3.0f), RECT4f(2.0f, 1.0f, 2.0f, 1.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4f_containsRect4f(RECT4f(3.0f, 0.0f, 3.0f, 0.0f), RECT4f(1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!contains, "Expected false but got true");
	
	contains = Rect4i_containsRect4i(RECT4i(0, 3, 0, 3), RECT4i(1, 2, 1, 2));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4i_containsRect4i(RECT4i(0, 2, 0, 2), RECT4i(1, 3, 1, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4i_containsRect4i(RECT4i(0, 1, 0, 1), RECT4i(1, 2, 1, 2));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4i_containsRect4i(RECT4i(0, 3, 0, 3), RECT4i(2, 1, 2, 1));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4i_containsRect4i(RECT4i(3, 0, 3, 0), RECT4i(1, 2, 1, 2));
	TestCase_assert(!contains, "Expected false but got true");
	
	contains = Rect4x_containsRect4x(RECT4x(0x00000, 0x30000, 0x00000, 0x30000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4x_containsRect4x(RECT4x(0x00000, 0x20000, 0x00000, 0x20000), RECT4x(0x10000, 0x30000, 0x10000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4x_containsRect4x(RECT4x(0x00000, 0x10000, 0x00000, 0x10000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4x_containsRect4x(RECT4x(0x00000, 0x30000, 0x00000, 0x30000), RECT4x(0x20000, 0x10000, 0x20000, 0x10000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4x_containsRect4x(RECT4x(0x30000, 0x00000, 0x30000, 0x00000), RECT4x(0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!contains, "Expected false but got true");
	
	contains = Box6d_containsBox6d(BOX6d(0.0, 3.0, 0.0, 3.0, 0.0, 3.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6d_containsBox6d(BOX6d(0.0, 2.0, 0.0, 2.0, 0.0, 2.0), BOX6d(1.0, 3.0, 1.0, 3.0, 1.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsBox6d(BOX6d(0.0, 1.0, 0.0, 1.0, 0.0, 1.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsBox6d(BOX6d(0.0, 3.0, 0.0, 3.0, 0.0, 3.0), BOX6d(2.0, 1.0, 2.0, 1.0, 2.0, 1.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsBox6d(BOX6d(3.0, 0.0, 3.0, 0.0, 3.0, 0.0), BOX6d(1.0, 2.0, 1.0, 2.0, 1.0, 2.0));
	TestCase_assert(!contains, "Expected false but got true");
	
	contains = Box6f_containsBox6f(BOX6f(0.0f, 3.0f, 0.0f, 3.0f, 0.0f, 3.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6f_containsBox6f(BOX6f(0.0f, 2.0f, 0.0f, 2.0f, 0.0f, 2.0f), BOX6f(1.0f, 3.0f, 1.0f, 3.0f, 1.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsBox6f(BOX6f(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsBox6f(BOX6f(0.0f, 3.0f, 0.0f, 3.0f, 0.0f, 3.0f), BOX6f(2.0f, 1.0f, 2.0f, 1.0f, 2.0f, 1.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsBox6f(BOX6f(3.0f, 0.0f, 3.0f, 0.0f, 3.0f, 0.0f), BOX6f(1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f));
	TestCase_assert(!contains, "Expected false but got true");
	
	contains = Box6i_containsBox6i(BOX6i(0, 3, 0, 3, 0, 3), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6i_containsBox6i(BOX6i(0, 2, 0, 2, 0, 2), BOX6i(1, 3, 1, 3, 1, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsBox6i(BOX6i(0, 1, 0, 1, 0, 1), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsBox6i(BOX6i(0, 3, 0, 3, 0, 3), BOX6i(2, 1, 2, 1, 2, 1));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsBox6i(BOX6i(3, 0, 3, 0, 3, 0), BOX6i(1, 2, 1, 2, 1, 2));
	TestCase_assert(!contains, "Expected false but got true");
	
	contains = Box6x_containsBox6x(BOX6x(0x00000, 0x30000, 0x00000, 0x30000, 0x00000, 0x30000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6x_containsBox6x(BOX6x(0x00000, 0x20000, 0x00000, 0x20000, 0x00000, 0x20000), BOX6x(0x10000, 0x30000, 0x10000, 0x30000, 0x10000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsBox6x(BOX6x(0x00000, 0x10000, 0x00000, 0x10000, 0x00000, 0x10000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsBox6x(BOX6x(0x00000, 0x30000, 0x00000, 0x30000, 0x00000, 0x30000), BOX6x(0x20000, 0x10000, 0x20000, 0x10000, 0x20000, 0x10000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsBox6x(BOX6x(0x30000, 0x00000, 0x30000, 0x00000, 0x30000, 0x00000), BOX6x(0x10000, 0x20000, 0x10000, 0x20000, 0x10000, 0x20000));
	TestCase_assert(!contains, "Expected false but got true");
}

static void testContainsVector() {
	bool contains;
	
	contains = Rect4d_containsVector2d(RECT4d(2.0, 4.0, 2.0, 4.0), VECTOR2d(3.0, 3.0));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4d_containsVector2d(RECT4d(2.0, 4.0, 2.0, 4.0), VECTOR2d(0.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4d_containsVector2d(RECT4d(2.0, 4.0, 2.0, 4.0), VECTOR2d(6.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4d_containsVector2d(RECT4d(2.0, 4.0, 2.0, 4.0), VECTOR2d(3.0, 0.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4d_containsVector2d(RECT4d(2.0, 4.0, 2.0, 4.0), VECTOR2d(3.0, 6.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4d_containsVector2d(RECT4d(-1.0, 1.0, 2.0, 4.0), VECTOR2d(0.0, 3.0));
	TestCase_assert(contains, "Expected true but got false");
	
	contains = Rect4f_containsVector2f(RECT4f(2.0f, 4.0f, 2.0f, 4.0f), VECTOR2f(3.0f, 3.0f));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4f_containsVector2f(RECT4f(2.0f, 4.0f, 2.0f, 4.0f), VECTOR2f(0.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4f_containsVector2f(RECT4f(2.0f, 4.0f, 2.0f, 4.0f), VECTOR2f(6.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4f_containsVector2f(RECT4f(2.0f, 4.0f, 2.0f, 4.0f), VECTOR2f(3.0f, 0.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4f_containsVector2f(RECT4f(2.0f, 4.0f, 2.0f, 4.0f), VECTOR2f(3.0f, 6.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4f_containsVector2f(RECT4f(-1.0f, 1.0f, 2.0f, 4.0f), VECTOR2f(0.0f, 3.0f));
	TestCase_assert(contains, "Expected true but got false");
	
	contains = Rect4i_containsVector2i(RECT4i(2, 4, 2, 4), VECTOR2i(3, 3));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4i_containsVector2i(RECT4i(2, 4, 2, 4), VECTOR2i(0, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4i_containsVector2i(RECT4i(2, 4, 2, 4), VECTOR2i(6, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4i_containsVector2i(RECT4i(2, 4, 2, 4), VECTOR2i(3, 0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4i_containsVector2i(RECT4i(2, 4, 2, 4), VECTOR2i(3, 6));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4i_containsVector2i(RECT4i(-1, 1, 2, 4), VECTOR2i(0, 3));
	TestCase_assert(contains, "Expected true but got false");
	
	contains = Rect4x_containsVector2x(RECT4x(0x20000, 0x40000, 0x20000, 0x40000), VECTOR2x(0x30000, 0x30000));
	TestCase_assert(contains, "Expected true but got false");
	contains = Rect4x_containsVector2x(RECT4x(0x20000, 0x40000, 0x20000, 0x40000), VECTOR2x(0x00000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4x_containsVector2x(RECT4x(0x20000, 0x40000, 0x20000, 0x40000), VECTOR2x(0x60000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4x_containsVector2x(RECT4x(0x20000, 0x40000, 0x20000, 0x40000), VECTOR2x(0x30000, 0x00000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4x_containsVector2x(RECT4x(0x20000, 0x40000, 0x20000, 0x40000), VECTOR2x(0x30000, 0x60000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Rect4x_containsVector2x(RECT4x(-0x10000, 0x10000, 0x20000, 0x40000), VECTOR2x(0x00000, 0x30000));
	TestCase_assert(contains, "Expected true but got false");
	
	contains = Box6d_containsVector3d(BOX6d(2.0, 4.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(3.0, 3.0, 3.0));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6d_containsVector3d(BOX6d(2.0, 4.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(0.0, 3.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsVector3d(BOX6d(2.0, 4.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(6.0, 3.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsVector3d(BOX6d(2.0, 4.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(3.0, 0.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsVector3d(BOX6d(2.0, 4.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(3.0, 6.0, 3.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsVector3d(BOX6d(2.0, 4.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(3.0, 3.0, 0.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsVector3d(BOX6d(2.0, 4.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(3.0, 3.0, 6.0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6d_containsVector3d(BOX6d(-1.0, 1.0, 2.0, 4.0, 2.0, 4.0), VECTOR3d(0.0, 3.0, 3.0));
	TestCase_assert(contains, "Expected true but got false");
	
	contains = Box6f_containsVector3f(BOX6f(2.0f, 4.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(3.0f, 3.0f, 3.0f));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6f_containsVector3f(BOX6f(2.0f, 4.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(0.0f, 3.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsVector3f(BOX6f(2.0f, 4.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(6.0f, 3.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsVector3f(BOX6f(2.0f, 4.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(3.0f, 0.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsVector3f(BOX6f(2.0f, 4.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(3.0f, 6.0f, 3.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsVector3f(BOX6f(2.0f, 4.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(3.0f, 3.0f, 0.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsVector3f(BOX6f(2.0f, 4.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(3.0f, 3.0f, 6.0f));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6f_containsVector3f(BOX6f(-1.0f, 1.0f, 2.0f, 4.0f, 2.0f, 4.0f), VECTOR3f(0.0f, 3.0f, 3.0f));
	TestCase_assert(contains, "Expected true but got false");
	
	contains = Box6i_containsVector3i(BOX6i(2, 4, 2, 4, 2, 4), VECTOR3i(3, 3, 3));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6i_containsVector3i(BOX6i(2, 4, 2, 4, 2, 4), VECTOR3i(0, 3, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsVector3i(BOX6i(2, 4, 2, 4, 2, 4), VECTOR3i(6, 3, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsVector3i(BOX6i(2, 4, 2, 4, 2, 4), VECTOR3i(3, 0, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsVector3i(BOX6i(2, 4, 2, 4, 2, 4), VECTOR3i(3, 6, 3));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsVector3i(BOX6i(2, 4, 2, 4, 2, 4), VECTOR3i(3, 3, 0));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsVector3i(BOX6i(2, 4, 2, 4, 2, 4), VECTOR3i(3, 3, 6));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6i_containsVector3i(BOX6i(-1, 1, 2, 4, 2, 4), VECTOR3i(0, 3, 3));
	TestCase_assert(contains, "Expected true but got false");
	
	contains = Box6x_containsVector3x(BOX6x(0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x30000, 0x30000, 0x30000));
	TestCase_assert(contains, "Expected true but got false");
	contains = Box6x_containsVector3x(BOX6x(0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x00000, 0x30000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsVector3x(BOX6x(0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x60000, 0x30000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsVector3x(BOX6x(0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x30000, 0x00000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsVector3x(BOX6x(0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x30000, 0x60000, 0x30000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsVector3x(BOX6x(0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x30000, 0x30000, 0x00000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsVector3x(BOX6x(0x20000, 0x40000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x30000, 0x30000, 0x60000));
	TestCase_assert(!contains, "Expected false but got true");
	contains = Box6x_containsVector3x(BOX6x(-0x10000, 0x10000, 0x20000, 0x40000, 0x20000, 0x40000), VECTOR3x(0x00000, 0x30000, 0x30000));
	TestCase_assert(contains, "Expected true but got false");
}

TEST_SUITE(RectBoxTest,
           testConstructor,
           testIntersection,
           testUnion,
           testOffset,
           testInset,
           testIsEmpty,
           testIntersectsRectBox,
           testContainsRectBox,
           testContainsVector)
