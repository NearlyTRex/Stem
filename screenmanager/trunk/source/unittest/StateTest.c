#include "unittest/TestSuite.h"
#include "statecontroller/State.h"

static void testInit() {
	State state;
	bool success;
	
	memset(&state, 0xFF, sizeof(state));
	success = State_init(&state);
	TestCase_assert(success, "Expected true but got false");
	TestCase_assert(state.dispose == State_dispose, "Expected %p but got %p", State_dispose, state.dispose);
	TestCase_assert(state.activate == State_activate, "Expected %p but got %p", State_activate, state.activate);
	TestCase_assert(state.deactivate == State_deactivate, "Expected %p but got %p", State_deactivate, state.deactivate);
	TestCase_assert(state.draw == State_draw, "Expected %p but got %p", State_draw, state.draw);
	TestCase_assert(state.stateController == NULL, "Expected NULL but got %p", state.stateController);
	State_dispose(&state);
}

TEST_SUITE(StateTest, testInit);
