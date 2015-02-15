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
	TestCase_assert(inputController->triggerAction == InputController_triggerAction, "Expected %p but got %p (%u)", InputController_triggerAction, inputController->triggerAction, lineNumber);
	TestCase_assert(inputController->releaseAction == InputController_releaseAction, "Expected %p but got %p (%u)", InputController_releaseAction, inputController->releaseAction, lineNumber);
}

static void testInit() {
	InputController inputController, * inputControllerPtr;
	InputMap * inputMap;
	
	memset(&inputController, 0x00, sizeof(InputController));
	InputController_init(&inputController, NULL, NULL);
	verifyInit(&inputController, NULL, __LINE__, NULL);
	InputController_dispose(&inputController);
	
	inputMap = InputMap_create();
	memset(&inputController, 0xFF, sizeof(InputController));
	InputController_init(&inputController, inputMap, "a", NULL);
	verifyInit(&inputController, inputMap, __LINE__, "a", NULL);
	InputController_dispose(&inputController);
	
	inputControllerPtr = InputController_create(inputMap, "b", "c", NULL);
	verifyInit(inputControllerPtr, inputMap, __LINE__, "b", "c", NULL);
	inputControllerPtr->dispose(inputControllerPtr);
	InputMap_dispose(inputMap);
}

static unsigned int actionDownCallCount;
static unsigned int actionUpCallCount;
static Atom lastActionID;

static bool actionDown(Atom eventID, void * eventData, void * context) {
	actionDownCallCount++;
	lastActionID = eventData;
	return true;
}

static bool actionUp(Atom eventID, void * eventData, void * context) {
	actionUpCallCount++;
	lastActionID = eventData;
	return true;
}

static void testActionTriggers() {
	InputController * inputController;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputController = InputController_create(NULL, "a", "b", NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	
	lastActionID = NULL;
	TestCase_assert(!InputController_isActionTriggered(inputController, ATOM("a")), "Expected false but got true");
	inputController->triggerAction(inputController, ATOM("a"));
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	TestCase_assert(InputController_isActionTriggered(inputController, ATOM("a")), "Expected true but got false");
	
	inputController->triggerAction(inputController, ATOM("a"));
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(InputController_isActionTriggered(inputController, ATOM("a")), "Expected true but got false");
	
	lastActionID = NULL;
	inputController->releaseAction(inputController, ATOM("a"));
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	TestCase_assert(!InputController_isActionTriggered(inputController, ATOM("a")), "Expected false but got true");
	
	lastActionID = NULL;
	inputController->triggerAction(inputController, ATOM("a"));
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	TestCase_assert(InputController_isActionTriggered(inputController, ATOM("a")), "Expected true but got false");
	
	lastActionID = NULL;
	TestCase_assert(!InputController_isActionTriggered(inputController, ATOM("b")), "Expected false but got true");
	inputController->triggerAction(inputController, ATOM("b"));
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	TestCase_assert(InputController_isActionTriggered(inputController, ATOM("b")), "Expected true but got false");
	
	InputController_dispose(inputController);
}

static void testKeyboardBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	InputMap_bindKey(inputMap, ATOM("a"), 1);
	InputMap_bindKey(inputMap, ATOM("b"), 2);
	
	lastActionID = NULL;
	InputController_keyDown(inputController, 1);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	InputController_keyDown(inputController, 1);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	InputController_keyUp(inputController, 1);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_keyDown(inputController, 1);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	InputController_keyDown(inputController, 2);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	InputController_dispose(inputController);
	InputMap_dispose(inputMap);
}

static void testKeyModifierBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	InputMap_bindKeyModifier(inputMap, ATOM("a"), 0x1);
	InputMap_bindKeyModifier(inputMap, ATOM("b"), 0x2);
	
	lastActionID = NULL;
	InputController_keyModifiersChanged(inputController, 0x1);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	InputController_keyModifiersChanged(inputController, 0x5);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	InputController_keyModifiersChanged(inputController, 0x4);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_keyModifiersChanged(inputController, 0x1);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	InputController_keyModifiersChanged(inputController, 0x3);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	InputController_dispose(inputController);
	InputMap_dispose(inputMap);
}

static void testButtonBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", "c", "d", NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	InputMap_bindButton(inputMap, ATOM("a"), 1, 2, 3);
	InputMap_bindButton(inputMap, ATOM("b"), 4, 2, 3);
	InputMap_bindButton(inputMap, ATOM("c"), 1, 5, 3);
	InputMap_bindButton(inputMap, ATOM("d"), 1, 2, 6);
	
	lastActionID = NULL;
	InputController_gamepadButtonDown(inputController, 1, 2, 3);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	InputController_gamepadButtonDown(inputController, 1, 2, 3);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	InputController_gamepadButtonUp(inputController, 1, 2, 3);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadButtonDown(inputController, 1, 2, 3);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	InputController_gamepadButtonDown(inputController, 4, 2, 3);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadButtonDown(inputController, 1, 5, 3);
	TestCase_assert(actionDownCallCount == 4, "Expected 4 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("c"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadButtonDown(inputController, 1, 2, 6);
	TestCase_assert(actionDownCallCount == 5, "Expected 5 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("d"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	InputController_dispose(inputController);
	InputMap_dispose(inputMap);
}

static void testAxisBindings() {
	InputController * inputController;
	InputMap * inputMap;
	
	actionDownCallCount = actionUpCallCount = 0;
	inputMap = InputMap_create();
	inputController = InputController_create(inputMap, "a", "b", "c", "d", "e", NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), actionDown, NULL);
	EventDispatcher_registerForEvent(inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), actionUp, NULL);
	InputMap_bindAxis(inputMap, ATOM("a"), 1, 2, 3, 0.75f, 0.25f);
	InputMap_bindAxis(inputMap, ATOM("b"), 4, 2, 3, 0.75f, 0.25f);
	InputMap_bindAxis(inputMap, ATOM("c"), 1, 5, 3, 0.75f, 0.25f);
	InputMap_bindAxis(inputMap, ATOM("d"), 1, 2, 6, 0.75f, 0.25f);
	InputMap_bindAxis(inputMap, ATOM("e"), 1, 2, 3, -0.75f, -0.25f);
	
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 1, "Expected 1 but got %u", actionDownCallCount);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 0.0f, 1.0f);
	TestCase_assert(actionUpCallCount == 1, "Expected 1 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 2, "Expected 2 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 4, 2, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 3, "Expected 3 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 5, 3, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 4, "Expected 4 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("c"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 6, 1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 5, "Expected 5 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("d"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, -1.0f, 0.0f);
	TestCase_assert(actionDownCallCount == 6, "Expected 6 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("e"), "Expected \"e\" (%p) but got \"%s\" (%p)", ATOM("b"), lastActionID, lastActionID);
	
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 0.0f, 1.0f);
	TestCase_assert(actionUpCallCount == 2, "Expected 2 but got %u", actionUpCallCount);
	InputMap_bindAxis(inputMap, ATOM("a"), 1, 2, 3, 1.0f, 0.0f);
	
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 0.875f, 0.0f);
	TestCase_assert(actionDownCallCount == 6, "Expected 6 but got %u", actionDownCallCount);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 1.0f, 0.875f);
	TestCase_assert(actionDownCallCount == 7, "Expected 7 but got %u", actionDownCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, 0.125f, 1.0f);
	TestCase_assert(actionUpCallCount == 2, "Expected 2 but got %u", actionUpCallCount);
	lastActionID = NULL;
	InputController_gamepadAxisMoved(inputController, 1, 2, 3, -0.125f, 0.125f);
	TestCase_assert(actionUpCallCount == 3, "Expected 3 but got %u", actionUpCallCount);
	TestCase_assert(lastActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastActionID, lastActionID);
	
	InputController_dispose(inputController);
	InputMap_dispose(inputMap);
}

TEST_SUITE(InputControllerTest,
           testInit,
           testActionTriggers,
           testKeyboardBindings,
           testKeyModifierBindings,
           testButtonBindings,
           testAxisBindings)
