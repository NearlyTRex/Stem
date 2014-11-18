#include "unittest/TestSuite.h"
#include "screenmanager/ScreenManager.h"

void testInit() {
	ScreenManager screenManager, * screenManagerPtr;
	bool success;
	
	memset(&screenManager, 0x00, sizeof(screenManager));
	success = ScreenManager_init(&screenManager);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(screenManager.dispose == ScreenManager_dispose, "Expected %p but got %p", ScreenManager_dispose, screenManager.dispose);
	TestCase_assert(screenManager.eventDispatcher != NULL, "Expected non-NULL but got NULL");
	screenManager.dispose(&screenManager);
	
	screenManagerPtr = ScreenManager_create();
	TestCase_assert(screenManagerPtr != NULL, "Expected non-NULL but got NULL");
	if (screenManagerPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(screenManagerPtr->dispose == ScreenManager_dispose, "Expected %p but got %p", ScreenManager_dispose, screenManagerPtr->dispose);
	TestCase_assert(screenManagerPtr->eventDispatcher != NULL, "Expected non-NULL but got NULL");
	screenManagerPtr->dispose(screenManagerPtr);
}

static void testTransitions() {
	ScreenManager * screenManager;
	Screen screen1, screen2, screen3;
	
	Screen_init(&screen1);
	Screen_init(&screen2);
	screenManager = ScreenManager_create();
	ScreenManager_addScreen(screenManager, &screen1);
	ScreenManager_addScreen(screenManager, &screen2);
	ScreenManager_addTransition(screenManager, &screen1, &screen2, "transition1");
	ScreenManager_addTransition(screenManager, &screen2, &screen1, "transition2");
	TestCase_assert(screenManager->currentScreen == NULL, "Expected NULL but got %p", screenManager->currentScreen);
	ScreenManager_setScreen(screenManager, &screen1);
	TestCase_assert(screenManager->currentScreen == &screen1, "Expected %p but got %p", &screen1, screenManager->currentScreen);
	ScreenManager_transition(screenManager, "transition1");
	TestCase_assert(screenManager->currentScreen == &screen2, "Expected %p but got %p", &screen2, screenManager->currentScreen);
	ScreenManager_transition(screenManager, "transition2");
	TestCase_assert(screenManager->currentScreen == &screen1, "Expected %p but got %p", &screen1, screenManager->currentScreen);
	ScreenManager_setScreen(screenManager, &screen2);
	TestCase_assert(screenManager->currentScreen == &screen2, "Expected %p but got %p", &screen2, screenManager->currentScreen);
	screenManager->dispose(screenManager);
	
	Screen_init(&screen3);
	screenManager = ScreenManager_create();
	ScreenManager_addScreen(screenManager, &screen1);
	ScreenManager_addScreen(screenManager, &screen2);
	ScreenManager_addScreen(screenManager, &screen3);
	ScreenManager_addTransition(screenManager, &screen1, &screen2, "transition");
	ScreenManager_addTransition(screenManager, &screen2, &screen3, "transition");
	ScreenManager_addTransition(screenManager, &screen3, &screen1, "transition1");
	ScreenManager_addTransition(screenManager, &screen3, &screen2, "transition2");
	TestCase_assert(screenManager->currentScreen == NULL, "Expected NULL but got %p", screenManager->currentScreen);
	ScreenManager_setScreen(screenManager, &screen1);
	TestCase_assert(screenManager->currentScreen == &screen1, "Expected %p but got %p", &screen1, screenManager->currentScreen);
	ScreenManager_transition(screenManager, "transition");
	TestCase_assert(screenManager->currentScreen == &screen2, "Expected %p but got %p", &screen2, screenManager->currentScreen);
	ScreenManager_transition(screenManager, "transition");
	TestCase_assert(screenManager->currentScreen == &screen3, "Expected %p but got %p", &screen3, screenManager->currentScreen);
	ScreenManager_transition(screenManager, "transition1");
	TestCase_assert(screenManager->currentScreen == &screen1, "Expected %p but got %p", &screen1, screenManager->currentScreen);
	ScreenManager_setScreen(screenManager, &screen3);
	TestCase_assert(screenManager->currentScreen == &screen3, "Expected %p but got %p", &screen3, screenManager->currentScreen);
	ScreenManager_transition(screenManager, "transition2");
	TestCase_assert(screenManager->currentScreen == &screen2, "Expected %p but got %p", &screen2, screenManager->currentScreen);
	screenManager->dispose(screenManager);
}

static void testInvalidTransitions() {
	ScreenManager * screenManager;
	Screen screen1, screen2;
	
	Screen_init(&screen1);
	Screen_init(&screen2);
	screenManager = ScreenManager_create();
	ScreenManager_setScreen(screenManager, &screen1);
	TestCase_assert(screenManager->currentScreen == NULL, "Expected NULL but got %p", screenManager->currentScreen);
	ScreenManager_addScreen(screenManager, &screen1);
	ScreenManager_setScreen(screenManager, &screen1);
	ScreenManager_addTransition(screenManager, &screen1, &screen2, "transition");
	ScreenManager_addTransition(screenManager, &screen2, &screen1, "transition");
	ScreenManager_addScreen(screenManager, &screen2);
	ScreenManager_transition(screenManager, "transition");
	TestCase_assert(screenManager->currentScreen == &screen1, "Expected %p but got %p", &screen1, screenManager->currentScreen);
	ScreenManager_setScreen(screenManager, &screen2);
	ScreenManager_transition(screenManager, "transition");
	TestCase_assert(screenManager->currentScreen == &screen2, "Expected %p but got %p", &screen2, screenManager->currentScreen);
	screenManager->dispose(screenManager);
}

struct TestScreen {
	Screen_structContents(struct TestScreen)
	unsigned int activateCalls;
	unsigned int deactivateCalls;
	unsigned int * callOrder;
};

static void testActivate(struct TestScreen * self) {
	self->activateCalls++;
	*self->callOrder <<= 2;
	*self->callOrder |= 1;
}

static void testDeactivate(struct TestScreen * self) {
	self->deactivateCalls++;
	*self->callOrder <<= 2;
	*self->callOrder |= 2;
}

static void testScreens() {
	ScreenManager * screenManager;
	struct TestScreen screen1, screen2;
	unsigned int callOrder = 0;
	
	Screen_init((Screen *) &screen1);
	screen1.activateCalls = screen1.deactivateCalls = 0;
	screen1.activate = testActivate;
	screen1.deactivate = testDeactivate;
	screen1.callOrder = &callOrder;
	screen2 = screen1;
	
	screenManager = ScreenManager_create();
	ScreenManager_addScreen(screenManager, (Screen *) &screen1);
	TestCase_assert(screen1.screenManager == screenManager, "Expected %p but got %p", screenManager, screen1.screenManager);
	ScreenManager_setScreen(screenManager, (Screen *) &screen1);
	TestCase_assert(screen1.activateCalls == 1, "Expected 1 but got %d", screen1.activateCalls);
	
	callOrder = 0;
	ScreenManager_addScreen(screenManager, (Screen *) &screen2);
	ScreenManager_addTransition(screenManager, (Screen *) &screen1, (Screen *) &screen2, "transition");
	ScreenManager_transition(screenManager, "transition");
	TestCase_assert(screen1.deactivateCalls == 1, "Expected 1 but got %d", screen1.deactivateCalls);
	TestCase_assert(screen2.activateCalls == 1, "Expected 1 but got %d", screen2.activateCalls);
	TestCase_assert(callOrder == 0x9, "Expected 0x9 but got 0x%X", callOrder);
	
	callOrder = 0;
	ScreenManager_setScreen(screenManager, (Screen *) &screen1);
	TestCase_assert(screen2.deactivateCalls == 1, "Expected 1 but got %d", screen2.deactivateCalls);
	TestCase_assert(screen1.activateCalls == 2, "Expected 2 but got %d", screen1.activateCalls);
	TestCase_assert(callOrder == 0x9, "Expected 0x9 but got 0x%X", callOrder);
}

TEST_SUITE(ScreenManagerTest, testInit, testTransitions, testInvalidTransitions, testScreens)
