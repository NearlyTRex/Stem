#include "unittest/TestSuite.h"
#include "inputcontroller/InputPlayback.h"
#include "utilities/IOUtilities.h"
#include "utilities/printfFormats.h"

static void verifyInit(InputPlayback * playback, InputController * controller, InputSession * session, unsigned int lineNumber) {
	TestCase_assert(playback != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(playback->inputController == controller, "Expected %p but got %p (%u)", controller, playback->inputController, lineNumber);
	TestCase_assert(playback->inputSession == session, "Expected %p but got %p (%u)", session, playback->inputSession, lineNumber);
	TestCase_assert(playback->eventDispatcher != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(playback->frameIndex == 0, "Expected 0 but got %u (%u)", playback->frameIndex, lineNumber);
	TestCase_assert(playback->dispose == InputPlayback_dispose, "Expected %p but got %p (%u)", InputPlayback_dispose, playback->dispose, lineNumber);
}

static void testInit() {
	InputPlayback playback, * playbackPtr;
	InputController * controller = (InputController *) 1;
	InputSession * session = InputSession_loadData("\x00\x00\x00\x00\x00\x00\x00\x00", 8, NULL);
	
	memset(&playback, 0x00, sizeof(playback));
	InputPlayback_init(&playback, NULL, NULL);
	verifyInit(&playback, NULL, NULL, __LINE__);
	InputPlayback_dispose(&playback);
	
	memset(&playback, 0xFF, sizeof(playback));
	InputPlayback_init(&playback, controller, session);
	verifyInit(&playback, controller, session, __LINE__);
	InputPlayback_dispose(&playback);
	
	playbackPtr = InputPlayback_create(controller, session);
	verifyInit(playbackPtr, controller, session, __LINE__);
	InputPlayback_dispose(playbackPtr);
	InputSession_dispose(session);
}

static unsigned int triggerActionCalls, releaseActionCalls;
static Atom lastTriggeredActionID, lastReleasedActionID;
static bool lastCallWasTrigger;

static bool testTriggerAction(InputController * self, Atom actionID) {
	triggerActionCalls++;
	lastTriggeredActionID = actionID;
	lastCallWasTrigger = true;
	return true;
}

static bool testReleaseAction(InputController * self, Atom actionID) {
	releaseActionCalls++;
	lastReleasedActionID = actionID;
	lastCallWasTrigger = false;
	return true;
}

static void testBasicPlayback() {
	InputSession * session;
	InputPlayback * playback;
	InputController testInputController;
	char testData1[] = "\x00\x00" "\x00\x00\x00\x00" "\x01\x00" "\x61\x00" "\x00\x00\x00\x00\x00\x00" "\x00\x00\x00\x00\x00\x00";
	char testData2[] = "\x00\x00" "\x00\x00\x00\x00" "\x02\x00" "\x61\x00" "\x62\x00" "\x00\x00\x00\x00\x01\x00" "\x01\x00\x00\x00\x00\x00" "\x02\x00\x00\x00\x01\x00";
	
	testInputController.triggerAction = testTriggerAction;
	testInputController.releaseAction = testReleaseAction;
	
	session = InputSession_loadData(testData1, sizeof(testData1) - 1, NULL);
	playback = InputPlayback_create(&testInputController, session);
	triggerActionCalls = releaseActionCalls = 0;
	
	InputPlayback_step(playback);
	TestCase_assert(triggerActionCalls == 1, "Expected 1 but got %u", triggerActionCalls);
	TestCase_assert(lastTriggeredActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastTriggeredActionID, lastTriggeredActionID);
	TestCase_assert(releaseActionCalls == 1, "Expected 1 but got %u", releaseActionCalls);
	TestCase_assert(lastReleasedActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastReleasedActionID, lastReleasedActionID);
	TestCase_assert(!lastCallWasTrigger, "Trigger and release called in the wrong sequence");
	
	InputPlayback_dispose(playback);
	InputSession_dispose(session);
	
	session = InputSession_loadData(testData2, sizeof(testData2) - 1, NULL);
	playback = InputPlayback_create(&testInputController, session);
	triggerActionCalls = releaseActionCalls = 0;
	
	InputPlayback_step(playback);
	TestCase_assert(triggerActionCalls == 1, "Expected 1 but got %u", triggerActionCalls);
	TestCase_assert(lastTriggeredActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastTriggeredActionID, lastTriggeredActionID);
	
	InputPlayback_step(playback);
	TestCase_assert(triggerActionCalls == 2, "Expected 2 but got %u", triggerActionCalls);
	TestCase_assert(lastTriggeredActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastTriggeredActionID, lastTriggeredActionID);
	
	InputPlayback_step(playback);
	TestCase_assert(triggerActionCalls == 2, "Expected 2 but got %u", triggerActionCalls);
	TestCase_assert(releaseActionCalls == 0, "Expected 0 but got %u", releaseActionCalls);
	
	InputPlayback_step(playback);
	TestCase_assert(releaseActionCalls == 1, "Expected 1 but got %u", releaseActionCalls);
	TestCase_assert(lastReleasedActionID == ATOM("b"), "Expected \"b\" (%p) but got \"%s\" (%p)", ATOM("b"), lastReleasedActionID, lastReleasedActionID);
	
	InputPlayback_dispose(playback);
	InputSession_dispose(session);
}

static unsigned int playbackCompleteCalls;

bool playbackComplete(Atom eventID, void * eventData, void * context) {
	playbackCompleteCalls++;
	return true;
}

static void testPlaybackCompleteEvent() {
	InputSession * session;
	InputPlayback * playback;
	InputController testInputController;
	char testData1[] = "\x00\x00" "\x00\x00\x00\x00" "\x01\x00" "\x61\x00" "\x01\x00\x00\x00\x00\x00";
	
	testInputController.triggerAction = testTriggerAction;
	testInputController.releaseAction = testReleaseAction;
	playbackCompleteCalls = 0;
	
	session = InputSession_loadData(testData1, sizeof(testData1) - 1, NULL);
	playback = InputPlayback_create(&testInputController, session);
	EventDispatcher_registerForEvent(playback->eventDispatcher, ATOM(INPUT_PLAYBACK_EVENT_PLAYBACK_COMPLETE), playbackComplete, NULL);
	
	InputPlayback_step(playback);
	TestCase_assert(playbackCompleteCalls == 0, "Expected 0 but got %u", playbackCompleteCalls);
	InputPlayback_step(playback);
	TestCase_assert(playbackCompleteCalls == 1, "Expected 1 but got %u", playbackCompleteCalls);
	InputPlayback_step(playback);
	TestCase_assert(playbackCompleteCalls == 1, "Expected 1 but got %u", playbackCompleteCalls);
	
	InputPlayback_dispose(playback);
	InputSession_dispose(session);
}

static void testRewind() {
	InputSession * session;
	InputPlayback * playback;
	InputController testInputController;
	char testData1[] = "\x00\x00" "\x00\x00\x00\x00" "\x01\x00" "\x61\x00" "\x00\x00\x00\x00\x00\x00" "\x00\x00\x00\x00\x00\x00";
	
	testInputController.triggerAction = testTriggerAction;
	testInputController.releaseAction = testReleaseAction;
	
	session = InputSession_loadData(testData1, sizeof(testData1) - 1, NULL);
	playback = InputPlayback_create(&testInputController, session);
	triggerActionCalls = releaseActionCalls = 0;
	
	InputPlayback_step(playback);
	TestCase_assert(triggerActionCalls == 1, "Expected 1 but got %u", triggerActionCalls);
	TestCase_assert(lastTriggeredActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastTriggeredActionID, lastTriggeredActionID);
	TestCase_assert(releaseActionCalls == 1, "Expected 1 but got %u", releaseActionCalls);
	TestCase_assert(lastReleasedActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastReleasedActionID, lastReleasedActionID);
	TestCase_assert(!lastCallWasTrigger, "Trigger and release called in the wrong sequence");
	
	InputPlayback_rewind(playback);
	InputPlayback_step(playback);
	TestCase_assert(triggerActionCalls == 2, "Expected 2 but got %u", triggerActionCalls);
	TestCase_assert(lastTriggeredActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastTriggeredActionID, lastTriggeredActionID);
	TestCase_assert(releaseActionCalls == 2, "Expected 2 but got %u", releaseActionCalls);
	TestCase_assert(lastReleasedActionID == ATOM("a"), "Expected \"a\" (%p) but got \"%s\" (%p)", ATOM("a"), lastReleasedActionID, lastReleasedActionID);
	TestCase_assert(!lastCallWasTrigger, "Trigger and release called in the wrong sequence");
	
	InputPlayback_dispose(playback);
	InputSession_dispose(session);
}

TEST_SUITE(InputPlaybackTest,
           testInit,
           testBasicPlayback,
           testPlaybackCompleteEvent,
           testRewind)
