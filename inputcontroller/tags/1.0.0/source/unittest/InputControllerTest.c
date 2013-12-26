#include "unittest/TestSuite.h"
#include "inputcontroller/InputController.h"

static void verifyInit(InputController * inputController, InputMap * inputMap, unsigned int lineNumber, ...) {
	va_list args;
	unsigned int actionCount = 0;
	const char * actionID;
	
	TestCase_assert(inputController != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputController->eventDispatcher != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputController->inputMap == inputMap, "Expected %p but got %p (%u)", inputMap, inputController->inputMap, lineNumber);
	TestCase_assert(inputController->lastModifiers == 0, "Expected 0 but got 0x%X (%u)", inputController->lastModifiers, lineNumber);
	va_start(args, lineNumber);
	while ((actionID = ATOM(va_arg(args, const char *))) != NULL) {
		TestCase_assert(inputController->actions[actionCount].actionID == actionID, "Expected \"%s\" (%p) but got \"%s\" (%p) (line %u, arg %u)", actionID, actionID, inputController->actions[actionCount].actionID, inputController->actions[actionCount].actionID, lineNumber, actionCount);
		TestCase_assert(!inputController->actions[actionCount].triggered, "Expected false but got true (line %u, arg %u)", lineNumber, actionCount);
		actionCount++;
	}
	va_end(args);
	TestCase_assert(inputController->actionCount == actionCount, "Expected %u but got %u (%u)", actionCount, inputController->actionCount, lineNumber);
	
	TestCase_assert(inputController->dispose == InputController_dispose, "Expected %p but got %p (%u)", InputController_dispose, inputController->dispose, lineNumber);
	TestCase_assert(inputController->keyDown == InputController_keyDown, "Expected %p but got %p (%u)", InputController_keyDown, inputController->keyDown, lineNumber);
	TestCase_assert(inputController->keyUp == InputController_keyUp, "Expected %p but got %p (%u)", InputController_keyUp, inputController->keyUp, lineNumber);
	TestCase_assert(inputController->keyModifiersChanged == InputController_keyModifiersChanged, "Expected %p but got %p (%u)", InputController_keyModifiersChanged, inputController->keyModifiersChanged, lineNumber);
	TestCase_assert(inputController->gamepadButtonDown == InputController_gamepadButtonDown, "Expected %p but got %p (%u)", InputController_gamepadButtonDown, inputController->gamepadButtonDown, lineNumber);
	TestCase_assert(inputController->gamepadButtonUp == InputController_gamepadButtonUp, "Expected %p but got %p (%u)", InputController_gamepadButtonUp, inputController->gamepadButtonUp, lineNumber);
	TestCase_assert(inputController->gamepadAxisMoved == InputController_gamepadAxisMoved, "Expected %p but got %p (%u)", InputController_gamepadAxisMoved, inputController->gamepadAxisMoved, lineNumber);
	TestCase_assert(inputController->triggerAction == InputController_triggerAction, "Expected %p but got %p (%u)", InputController_triggerAction, inputController->triggerAction, lineNumber);
	TestCase_assert(inputController->releaseAction == InputController_releaseAction, "Expected %p but got %p (%u)", InputController_releaseAction, inputController->releaseAction, lineNumber);
}

static void testInit() {
	InputController inputController, * inputControllerPtr;
	InputMap * inputMap;
	
	memset(&inputController, 0x00, sizeof(InputController));
	InputController_init(&inputController, NULL, NULL);
	verifyInit(&inputController, NULL, __LINE__, NULL);
	inputController.dispose(&inputController);
	
	inputMap = InputMap_create();
	memset(&inputController, 0xFF, sizeof(InputController));
	InputController_init(&inputController, inputMap, "a", NULL);
	verifyInit(&inputController, inputMap, __LINE__, "a", NULL);
	inputController.dispose(&inputController);
	
	inputControllerPtr = InputController_create(inputMap, "b", "c", NULL);
	verifyInit(inputControllerPtr, inputMap, __LINE__, "b", "c", NULL);
	inputControllerPtr->dispose(inputControllerPtr);
	inputMap->dispose(inputMap);
}

static unsigned int actionDownCallCount;
static unsigned int actionUpCallCount;
static Atom lastActionID;

static bool actionDown(void * sender, Atom eventID, void * eventData, void * context) {
	actionDownCallCount++;
	lastActionID = eventData;
	return true;
}

static bool actionUp(void * sender, Atom eventID, void * eventData, void * context) {
	actionUpCallCount++;
	lastActionID = eventData;
	return true;
}

static void testActionTriggers() {
	InputController * inputController;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputController = InputController_create(NULL, "a", "b", NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	
	lastActionID = NULL;
	inputController->triggerAction(inputController, ATOM("a"));
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	inputController->triggerAction(inputController, ATOM("a"));
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	inputController->releaseAction(inputController, ATOM("a"));
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->triggerAction(inputController, ATOM("a"));
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	inputController->triggerAction(inputController, ATOM("b"));
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	inputController->dispose(inputController);
}

static void testKeyboardBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	inputMap->bindKey(inputMap, ATOM("a"), 1, 0);
	inputMap->bindKey(inputMap, ATOM("b"), 2, 0);
	
	lastActionID = NULL;
	inputController->keyDown(inputController, 1);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	inputController->keyDown(inputController, 1);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	inputController->keyUp(inputController, 1);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->keyDown(inputController, 1);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	inputController->keyDown(inputController, 2);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	inputController->dispose(inputController);
	inputMap->dispose(inputMap);
}

static void testKeyModifierBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	inputMap->bindKeyModifier(inputMap, ATOM("a"), 0x1);
	inputMap->bindKeyModifier(inputMap, ATOM("b"), 0x2);
	
	lastActionID = NULL;
	inputController->keyModifiersChanged(inputController, 0x1);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	inputController->keyModifiersChanged(inputController, 0x5);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	inputController->keyModifiersChanged(inputController, 0x4);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->keyModifiersChanged(inputController, 0x1);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	inputController->keyModifiersChanged(inputController, 0x3);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	inputController->dispose(inputController);
	inputMap->dispose(inputMap);
}

static void testButtonBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", "c", "d", NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	inputMap->bindButton(inputMap, ATOM("a"), 1, 2, 3);
	inputMap->bindButton(inputMap, ATOM("b"), 4, 2, 3);
	inputMap->bindButton(inputMap, ATOM("c"), 1, 5, 3);
	inputMap->bindButton(inputMap, ATOM("d"), 1, 2, 6);
	
	lastActionID = NULL;
	inputController->gamepadButtonDown(inputController, 1, 2, 3);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	inputController->gamepadButtonDown(inputController, 1, 2, 3);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	inputController->gamepadButtonUp(inputController, 1, 2, 3);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadButtonDown(inputController, 1, 2, 3);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	inputController->gamepadButtonDown(inputController, 4, 2, 3);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadButtonDown(inputController, 1, 5, 3);
	TestCase_assert(actionDownCallCount == 4, "Expected 4 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("c"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadButtonDown(inputController, 1, 2, 6);
	TestCase_assert(actionDownCallCount == 5, "Expected 5 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("d"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	inputController->dispose(inputController);
	inputMap->dispose(inputMap);
}

static void testAxisBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", "c", "d", "e", NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	inputController->eventDispatcher->registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	inputMap->bindAxis(inputMap, ATOM("a"), 1, 2, 3, 0.75f, 0.25f);
	inputMap->bindAxis(inputMap, ATOM("b"), 4, 2, 3, 0.75f, 0.25f);
	inputMap->bindAxis(inputMap, ATOM("c"), 1, 5, 3, 0.75f, 0.25f);
	inputMap->bindAxis(inputMap, ATOM("d"), 1, 2, 6, 0.75f, 0.25f);
	inputMap->bindAxis(inputMap, ATOM("e"), 1, 2, 3, -0.75f, -0.25f);
	
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 0.0f, 1.0f);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 4, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 5, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 4, "Expected 4 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("c"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 6, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 5, "Expected 5 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("d"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, -1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 6, "Expected 6 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("e"), "Expected \"e\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 0.0f, 1.0f);
	TestCase_assert(actionUpCallCount == 2, "Expected 2 but got %u", actionUpCallCount);
	inputMap->bindAxis(inputMap, ATOM("a"), 1, 2, 3, 1.0f, 0.0f);
	
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 0.875f, 0.0f);
	TestCase_assert(actionDownCallCount == 6, "Expected 6 but got %u", actionDownCallCount);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.875f);
	TestCase_assert(actionDownCallCount == 7, "Expected 7 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, 0.125f, 1.0f);
	TestCase_assert(actionUpCallCount == 2, "Expected 2 but got %u", actionUpCallCount);
	lastActionID = NULL;
	inputController->gamepadAxisMoved(inputController, 1, 2, 3, -0.125f, 0.125f);
	TestCase_assert(actionUpCallCount == 3, "Expected 3 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	inputController->dispose(inputController);
	inputMap->dispose(inputMap);
}

TEST_SUITE(InputControllerTest,
           testInit,
           testActionTriggers,
           testKeyboardBindings,
           testKeyModifierBindings,
           testButtonBindings,
           testAxisBindings)
