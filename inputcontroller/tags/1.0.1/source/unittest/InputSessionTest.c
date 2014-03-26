#include "inputcontroller/InputSession.h"
#include "unittest/TestSuite.h"
#include "utilities/IOUtilities.h"
#include "utilities/printfFormats.h"
#include <stdbool.h>
#include <unistd.h>

static const char testData1[] = \
"\x00\x00" // formatVersion
"\x00\x00\x00\x00" // replayStartupDataSize
"\x00\x00"; // actionCount

static void verifyTest1(InputSession * inputSession, unsigned int lineNumber) {
	TestCase_assert(inputSession != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputSession->replayStartupDataSize == 0, "Expected 0 but got " SIZE_T_FORMAT " (%u)", inputSession->replayStartupDataSize, lineNumber);
	TestCase_assert(inputSession->actionCount == 0, "Expected 0 but got %u (%u)", inputSession->actionCount, lineNumber);
	TestCase_assert(inputSession->eventCount == 0, "Expected 0 but got %u (%u)", inputSession->eventCount, lineNumber);
}

static const char testData2[] = \
"\x00\x00" // formatVersion
"\x04\x00\x00\x00" // replayStartupDataSize
"\x01\x02\x03\x04" // replayStartupData
"\x03\x00" // actionCount
"\x61\x00" // "a"
"\x62\x63\x00" // "bc"
"\x66\x6F\x6F\x00" // "foo"
"\x00\x00\x00\x00" "\x00\x00" // Event 0
"\x01\x00\x00\x00" "\x01\x00" // Event 1
"\x00\x00\x00\x01" "\x02\x00" // Event 2
"\x00\x00\x00\x00" "\x00\x00"; // Event 3

static void verifyTest2(InputSession * inputSession, unsigned int lineNumber) {
	TestCase_assert(inputSession != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputSession->replayStartupDataSize == 4, "Expected 4 but got " SIZE_T_FORMAT " (%u)", inputSession->replayStartupDataSize, lineNumber);
	TestCase_assert(inputSession->replayStartupData != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(!memcmp(inputSession->replayStartupData, "\x01\x02\x03\x04", 4), "Expected 01 02 03 04 but got %02X %02X %02X %02X (%u)", ((char *) inputSession->replayStartupData)[0], ((char *) inputSession->replayStartupData)[1], ((char *) inputSession->replayStartupData)[2], ((char *) inputSession->replayStartupData)[3], lineNumber);
	TestCase_assert(inputSession->actionCount == 3, "Expected 3 but got %u (%u)", inputSession->actionCount, lineNumber);
	TestCase_assert(inputSession->actions != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputSession->eventCount == 4, "Expected 4 but got %u (%u)", inputSession->eventCount, lineNumber);
	TestCase_assert(inputSession->events != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputSession->events[0].frameDelta == 0, "Expected 0 but got %u (%u)", inputSession->events[0].frameDelta, lineNumber);
	TestCase_assert(inputSession->events[0].actionIndex == 0, "Expected 0 but got %u (%u)", inputSession->events[0].actionIndex, lineNumber);
	TestCase_assert(inputSession->events[1].frameDelta == 1, "Expected 0 but got %u (%u)", inputSession->events[1].frameDelta, lineNumber);
	TestCase_assert(inputSession->events[1].actionIndex == 1, "Expected 0 but got %u (%u)", inputSession->events[1].actionIndex, lineNumber);
	TestCase_assert(inputSession->events[2].frameDelta == 16777216, "Expected 16777216 but got %u (%u)", inputSession->events[2].frameDelta, lineNumber);
	TestCase_assert(inputSession->events[2].actionIndex == 2, "Expected 2 but got %u (%u)", inputSession->events[2].actionIndex, lineNumber);
	TestCase_assert(inputSession->events[3].frameDelta == 0, "Expected 0 but got %u (%u)", inputSession->events[3].frameDelta, lineNumber);
	TestCase_assert(inputSession->events[3].actionIndex == 0, "Expected 0 but got %u (%u)", inputSession->events[3].actionIndex, lineNumber);
}

static void testLoadData() {
	InputSession * inputSession;
	
	inputSession = InputSession_loadData(testData1, sizeof(testData1) - 1, NULL);
	verifyTest1(inputSession, __LINE__);
	InputSession_dispose(inputSession);
	
	inputSession = InputSession_loadData(testData2, sizeof(testData2) - 1, NULL);
	verifyTest2(inputSession, __LINE__);
	InputSession_dispose(inputSession);
}

static void testLoadFile() {
	InputSession * inputSession;
	const char * tempFilePath;
	int fd;
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	write(fd, testData1, sizeof(testData1) - 1);
	close(fd);
	inputSession = InputSession_loadFile(tempFilePath, NULL);
	unlink(tempFilePath);
	verifyTest1(inputSession, __LINE__);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &fd);
	write(fd, testData2, sizeof(testData2) - 1);
	close(fd);
	inputSession = InputSession_loadFile(tempFilePath, NULL);
	unlink(tempFilePath);
	verifyTest2(inputSession, __LINE__);
}

static void testErrorHandling() {
	InputSession * inputSession;
	enum InputSessionError error;
	char testDataFormatTooNew[] = "\x00\x01" "\x00\x00\x00\x00" "\x00\x00";
	char testDataPrematureEOF1[] = "\x00";
	char testDataPrematureEOF2[] = "\x00\x00" "\x00\x00\x00";
	char testDataPrematureEOF3[] = "\x00\x00" "\x00\x00\x00\x00" "\x00";
	char testDataPrematureEOF4[] = "\x00\x00" "\x00\x00\x00\x00" "\x01\x00";
	char testDataPrematureEOF5[] = "\x00\x00" "\x00\x00\x00\x00" "\x01\x00" "\x61";
	char testDataPrematureEOF6[] = "\x00\x00" "\x04\x00\x00\x00" "\x00\x00";
	char testDataInvalidActionIndex[] = "\x00\x00" "\x00\x00\x00\x00" "\x00\x00" "\x00\x00\x00\x00\x00\x00";
	
	error = -1;
	inputSession = InputSession_loadFile("If this file exists, unit tests will fail", &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_FILE_NOT_FOUND, "Expected %d but got %d", INPUT_SESSION_ERROR_FILE_NOT_FOUND, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataFormatTooNew, sizeof(testDataFormatTooNew) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_FORMAT_TOO_NEW, "Expected %d but got %d", INPUT_SESSION_ERROR_FORMAT_TOO_NEW, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataPrematureEOF1, sizeof(testDataPrematureEOF1) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_PREMATURE_EOF, "Expected %d but got %d", INPUT_SESSION_ERROR_PREMATURE_EOF, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataPrematureEOF2, sizeof(testDataPrematureEOF2) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_PREMATURE_EOF, "Expected %d but got %d", INPUT_SESSION_ERROR_PREMATURE_EOF, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataPrematureEOF3, sizeof(testDataPrematureEOF3) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_PREMATURE_EOF, "Expected %d but got %d", INPUT_SESSION_ERROR_PREMATURE_EOF, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataPrematureEOF4, sizeof(testDataPrematureEOF4) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_PREMATURE_EOF, "Expected %d but got %d", INPUT_SESSION_ERROR_PREMATURE_EOF, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataPrematureEOF5, sizeof(testDataPrematureEOF5) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_PREMATURE_EOF, "Expected %d but got %d", INPUT_SESSION_ERROR_PREMATURE_EOF, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataPrematureEOF6, sizeof(testDataPrematureEOF6) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_PREMATURE_EOF, "Expected %d but got %d", INPUT_SESSION_ERROR_PREMATURE_EOF, error);
	
	error = -1;
	inputSession = InputSession_loadData(testDataInvalidActionIndex, sizeof(testDataInvalidActionIndex) - 1, &error);
	TestCase_assert(inputSession == NULL, "Expected NULL but got %p", inputSession);
	TestCase_assert(error == INPUT_SESSION_ERROR_INVALID_ACTION_INDEX, "Expected %d but got %d", INPUT_SESSION_ERROR_INVALID_ACTION_INDEX, error);
}

TEST_SUITE(InputSessionTest,
           testLoadData,
           testLoadFile,
           testErrorHandling)
