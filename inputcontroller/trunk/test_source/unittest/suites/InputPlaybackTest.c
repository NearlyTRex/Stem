#include "unittest/framework/TestSuite.h"
#include "inputcontroller/InputPlayback.h"
#include "utilities/IOUtilities.h"

#if defined(WIN32)
#define SIZE_T_FORMAT "%Iu"
#else
#define SIZE_T_FORMAT "%zu"
#endif

static void verifyInit(InputPlayback * playback, InputController * controller, InputSession * session, unsigned int lineNumber) {
	TestCase_assert(playback != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(playback->inputController == controller, "Expected %p but got %p (%u)", controller, playback->inputController, lineNumber);
	TestCase_assert(playback->inputSession == session, "Expected %p but got %p (%u)", session, playback->inputSession, lineNumber);
	TestCase_assert(playback->eventDispatcher != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(playback->frameIndex == 0, "Expected 0 but got %u (%u)", playback->frameIndex, lineNumber);
	TestCase_assert(playback->dispose == InputPlayback_dispose, "Expected %p but got %p (%u)", InputPlayback_dispose, playback->dispose, lineNumber);
	TestCase_assert(playback->step == InputPlayback_step, "Expected %p but got %p (%u)", InputPlayback_step, playback->step, lineNumber);
	TestCase_assert(playback->rewind == InputPlayback_rewind, "Expected %p but got %p (%u)", InputPlayback_rewind, playback->rewind, lineNumber);
}

static void testInit() {
	InputPlayback playback, * playbackPtr;
	InputController * controller = (InputController *) 1;
	InputSession * session = (InputSession *) 2;
	
	memset(&playback, 0x00, sizeof(playback));
	InputPlayback_init(&playback, NULL, NULL);
	verifyInit(&playback, NULL, NULL, __LINE__);
	playback.dispose(&playback);
	
	memset(&playback, 0xFF, sizeof(playback));
	InputPlayback_init(&playback, controller, session);
	verifyInit(&playback, controller, session, __LINE__);
	playback.dispose(&playback);
	
	playbackPtr = InputPlayback_create(controller, session);
	verifyInit(playbackPtr, controller, session, __LINE__);
	playbackPtr->dispose(playbackPtr);
}

static void testBasicPlayback() {
	//InputSession * session;
	//InputPlayback * playback;
	//char testData1[] = "";
	
	//session = InputSession_loadData(testData1, sizeof(testData1), NULL);
	//playback = InputPlayback_create(NULL, session);
	TestCase_assert(false, "Unimplemented");
}

static void testRewind() {
	TestCase_assert(false, "Unimplemented");
}

static void testPlaybackCompleteEvent() {
	TestCase_assert(false, "Unimplemented");
}

TEST_SUITE(InputPlaybackTest,
           testInit,
           testBasicPlayback,
           testRewind,
           testPlaybackCompleteEvent)
