#include "unittest/TestSuite.h"
#include "screenmanager/Screen.h"

static void testInit() {
	Screen screen;
	bool success;
	
	memset(&screen, 0xFF, sizeof(screen));
	success = Screen_init(&screen);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(screen.dispose == Screen_dispose, "Expected %p but got %p", Screen_dispose, screen.dispose);
	TestCase_assert(screen.activate == Screen_activate, "Expected %p but got %p", Screen_activate, screen.activate);
	TestCase_assert(screen.deactivate == Screen_deactivate, "Expected %p but got %p", Screen_deactivate, screen.deactivate);
	TestCase_assert(screen.screenManager == NULL, "Expected NULL but got %p", screen.screenManager);
	Screen_dispose(&screen);
}

TEST_SUITE(ScreenTest, testInit);
