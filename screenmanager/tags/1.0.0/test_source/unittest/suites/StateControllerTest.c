#include "unittest/framework/TestSuite.h"
#include "statecontroller/StateController.h"

void testInit() {
	StateController stateController, * stateControllerPtr;
	
	memset(&stateController, 0x00, sizeof(stateController));
	StateController_init(&stateController);
	TestCase_assert(stateController.dispose == StateController_dispose, "Expected %p but got %p", StateController_dispose, stateController.dispose);
	TestCase_assert(stateController.addState == StateController_addState, "Expected %p but got %p", StateController_addState, stateController.addState);
	TestCase_assert(stateController.setState == StateController_setState, "Expected %p but got %p", StateController_setState, stateController.setState);
	TestCase_assert(stateController.addTransition == StateController_addTransition, "Expected %p but got %p", StateController_addTransition, stateController.addTransition);
	TestCase_assert(stateController.transition == StateController_transition, "Expected %p but got %p", StateController_transition, stateController.transition);
	TestCase_assert(stateController.eventDispatcher != NULL, "Expected non-NULL but got NULL");
	stateController.dispose(&stateController);
	
	stateControllerPtr = StateController_create();
	TestCase_assert(stateControllerPtr != NULL, "Expected non-NULL but got NULL");
	if (stateControllerPtr == NULL) {return;} // Suppress clang warning
	TestCase_assert(stateControllerPtr->dispose == StateController_dispose, "Expected %p but got %p", StateController_dispose, stateControllerPtr->dispose);
	TestCase_assert(stateControllerPtr->addState == StateController_addState, "Expected %p but got %p", StateController_addState, stateControllerPtr->addState);
	TestCase_assert(stateControllerPtr->setState == StateController_setState, "Expected %p but got %p", StateController_setState, stateControllerPtr->setState);
	TestCase_assert(stateControllerPtr->addTransition == StateController_addTransition, "Expected %p but got %p", StateController_addTransition, stateControllerPtr->addTransition);
	TestCase_assert(stateControllerPtr->transition == StateController_transition, "Expected %p but got %p", StateController_transition, stateControllerPtr->transition);
	TestCase_assert(stateControllerPtr->eventDispatcher != NULL, "Expected non-NULL but got NULL");
	stateControllerPtr->dispose(stateControllerPtr);
}

static void testTransitions() {
	StateController * stateController;
	State state1, state2, state3;
	
	State_init(&state1);
	State_init(&state2);
	stateController = StateController_create();
	stateController->addState(stateController, &state1);
	stateController->addState(stateController, &state2);
	stateController->addTransition(stateController, &state1, &state2, "transition1");
	stateController->addTransition(stateController, &state2, &state1, "transition2");
	TestCase_assert(stateController->currentState == NULL, "Expected NULL but got %p", stateController->currentState);
	stateController->setState(stateController, &state1);
	TestCase_assert(stateController->currentState == &state1, "Expected %p but got %p", &state1, stateController->currentState);
	stateController->transition(stateController, "transition1");
	TestCase_assert(stateController->currentState == &state2, "Expected %p but got %p", &state2, stateController->currentState);
	stateController->transition(stateController, "transition2");
	TestCase_assert(stateController->currentState == &state1, "Expected %p but got %p", &state1, stateController->currentState);
	stateController->setState(stateController, &state2);
	TestCase_assert(stateController->currentState == &state2, "Expected %p but got %p", &state2, stateController->currentState);
	stateController->dispose(stateController);
	
	State_init(&state3);
	stateController = StateController_create();
	stateController->addState(stateController, &state1);
	stateController->addState(stateController, &state2);
	stateController->addState(stateController, &state3);
	stateController->addTransition(stateController, &state1, &state2, "transition");
	stateController->addTransition(stateController, &state2, &state3, "transition");
	stateController->addTransition(stateController, &state3, &state1, "transition1");
	stateController->addTransition(stateController, &state3, &state2, "transition2");
	TestCase_assert(stateController->currentState == NULL, "Expected NULL but got %p", stateController->currentState);
	stateController->setState(stateController, &state1);
	TestCase_assert(stateController->currentState == &state1, "Expected %p but got %p", &state1, stateController->currentState);
	stateController->transition(stateController, "transition");
	TestCase_assert(stateController->currentState == &state2, "Expected %p but got %p", &state2, stateController->currentState);
	stateController->transition(stateController, "transition");
	TestCase_assert(stateController->currentState == &state3, "Expected %p but got %p", &state3, stateController->currentState);
	stateController->transition(stateController, "transition1");
	TestCase_assert(stateController->currentState == &state1, "Expected %p but got %p", &state1, stateController->currentState);
	stateController->setState(stateController, &state3);
	TestCase_assert(stateController->currentState == &state3, "Expected %p but got %p", &state3, stateController->currentState);
	stateController->transition(stateController, "transition2");
	TestCase_assert(stateController->currentState == &state2, "Expected %p but got %p", &state2, stateController->currentState);
	stateController->dispose(stateController);
}

static void testInvalidTransitions() {
	StateController * stateController;
	State state1, state2;
	
	State_init(&state1);
	State_init(&state2);
	stateController = StateController_create();
	stateController->setState(stateController, &state1);
	TestCase_assert(stateController->currentState == NULL, "Expected NULL but got %p", stateController->currentState);
	stateController->addState(stateController, &state1);
	stateController->setState(stateController, &state1);
	stateController->addTransition(stateController, &state1, &state2, "transition");
	stateController->addTransition(stateController, &state2, &state1, "transition");
	stateController->addState(stateController, &state2);
	stateController->transition(stateController, "transition");
	TestCase_assert(stateController->currentState == &state1, "Expected %p but got %p", &state1, stateController->currentState);
	stateController->setState(stateController, &state2);
	stateController->transition(stateController, "transition");
	TestCase_assert(stateController->currentState == &state2, "Expected %p but got %p", &state2, stateController->currentState);
	stateController->dispose(stateController);
}

struct TestState {
	State_structContents
	unsigned int activateCalls;
	unsigned int deactivateCalls;
	unsigned int * callOrder;
};

static void testActivate(void * selfPtr) {
	struct TestState * self = selfPtr;
	self->activateCalls++;
	*self->callOrder <<= 2;
	*self->callOrder |= 1;
}

static void testDeactivate(void * selfPtr) {
	struct TestState * self = selfPtr;
	self->deactivateCalls++;
	*self->callOrder <<= 2;
	*self->callOrder |= 2;
}

static void testStates() {
	StateController * stateController;
	struct TestState state1, state2;
	unsigned int callOrder = 0;
	
	State_init((State *) &state1);
	state1.activateCalls = state1.deactivateCalls = 0;
	state1.activate = testActivate;
	state1.deactivate = testDeactivate;
	state1.callOrder = &callOrder;
	state2 = state1;
	
	stateController = StateController_create();
	stateController->addState(stateController, (State *) &state1);
	TestCase_assert(state1.stateController == stateController, "Expected %p but got %p", stateController, state1.stateController);
	stateController->setState(stateController, (State *) &state1);
	TestCase_assert(state1.activateCalls == 1, "Expected 1 but got %d", state1.activateCalls);
	
	callOrder = 0;
	stateController->addState(stateController, (State *) &state2);
	stateController->addTransition(stateController, (State *) &state1, (State *) &state2, "transition");
	stateController->transition(stateController, "transition");
	TestCase_assert(state1.deactivateCalls == 1, "Expected 1 but got %d", state1.deactivateCalls);
	TestCase_assert(state2.activateCalls == 1, "Expected 1 but got %d", state2.activateCalls);
	TestCase_assert(callOrder == 0x9, "Expected 0x9 but got 0x%X", callOrder);
	
	callOrder = 0;
	stateController->setState(stateController, (State *) &state1);
	TestCase_assert(state2.deactivateCalls == 1, "Expected 1 but got %d", state2.deactivateCalls);
	TestCase_assert(state1.activateCalls == 2, "Expected 2 but got %d", state1.activateCalls);
	TestCase_assert(callOrder == 0x9, "Expected 0x9 but got 0x%X", callOrder);
}

TEST_SUITE(StateControllerTest, testInit, testTransitions, testInvalidTransitions, testStates)
