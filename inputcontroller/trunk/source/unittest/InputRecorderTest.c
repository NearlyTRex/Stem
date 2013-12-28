#include "inputcontroller/InputRecorder.h"
#include "unittest/TestSuite.h"
#include "utilities/IOUtilities.h"
#include "utilities/printfFormats.h"
#include <stdbool.h>
#include <unistd.h>

static void verifyInit(InputRecorder * inputRecorder, InputController * inputController, bool fileOutput, unsigned int lineNumber) {
	TestCase_assert(inputRecorder != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	TestCase_assert(inputRecorder->dispose == InputRecorder_dispose, "Expected %p but got %p (%u)", InputRecorder_dispose, inputRecorder->dispose, lineNumber);
	TestCase_assert(inputRecorder->nextFrame == InputRecorder_nextFrame, "Expected %p but got %p (%u)", InputRecorder_nextFrame, inputRecorder->nextFrame, lineNumber);
	TestCase_assert(inputRecorder->inputController == inputController, "Expected %p but got %p (%u)", inputController, inputRecorder->inputController, lineNumber);
	TestCase_assert(inputRecorder->frameIndex == 0, "Expected 0 but got %u (%u)", inputRecorder->frameIndex, lineNumber);
	if (fileOutput) {
		TestCase_assert(inputRecorder->outputFile != NULL, "Expected non-NULL but got NULL (%u)", lineNumber);
	} else {
		TestCase_assert(inputRecorder->outputFile == NULL, "Expected NULL but got %p (%u)", inputRecorder->outputFile, lineNumber);
		TestCase_assert(inputRecorder->memwriteContext.realloc, "Expected true but got false (%u)", lineNumber);
	}
}

static void testInit() {
	InputRecorder inputRecorder, * inputRecorderPtr;
	InputController * inputController;
	const char * tempFilePath;
	int tempFD;
	
	inputController = InputController_create(NULL, NULL);
	
	memset(&inputRecorder, 0x00, sizeof(InputRecorder));
	InputRecorder_initWithMemwriteOutput(&inputRecorder, NULL, NULL, 0);
	verifyInit(&inputRecorder, NULL, NULL, __LINE__);
	inputRecorder.dispose(&inputRecorder);
	
	memset(&inputRecorder, 0xFF, sizeof(InputRecorder));
	InputRecorder_initWithMemwriteOutput(&inputRecorder, inputController, NULL, 0);
	verifyInit(&inputRecorder, inputController, NULL, __LINE__);
	inputRecorder.dispose(&inputRecorder);
	
	inputRecorderPtr = InputRecorder_createWithMemwriteOutput(NULL, NULL, 0);
	verifyInit(inputRecorderPtr, NULL, NULL, __LINE__);
	inputRecorderPtr->dispose(inputRecorderPtr);
	
	tempFilePath = temporaryFilePath("tmpXXXXXX", &tempFD);
	close(tempFD);
	
	memset(&inputRecorder, 0x00, sizeof(InputRecorder));
	InputRecorder_initWithFileOutput(&inputRecorder, NULL, NULL, 0, tempFilePath);
	verifyInit(&inputRecorder, NULL, tempFilePath, __LINE__);
	inputRecorder.dispose(&inputRecorder);
	
	memset(&inputRecorder, 0xFF, sizeof(InputRecorder));
	InputRecorder_initWithFileOutput(&inputRecorder, inputController, NULL, 0, tempFilePath);
	verifyInit(&inputRecorder, inputController, tempFilePath, __LINE__);
	inputRecorder.dispose(&inputRecorder);
	
	inputRecorderPtr = InputRecorder_createWithFileOutput(NULL, NULL, 0, tempFilePath);
	verifyInit(inputRecorderPtr, NULL, tempFilePath, __LINE__);
	inputRecorderPtr->dispose(inputRecorderPtr);
	
	inputController->dispose(inputController);
	unlink(tempFilePath);
}

static void testMemwriteRecording() {
	InputRecorder * inputRecorder;
	InputController * inputController;
	
	inputController = InputController_create(NULL, NULL);
	
	inputRecorder = InputRecorder_createWithMemwriteOutput(inputController, NULL, 0);
	TestCase_assert(inputRecorder->memwriteContext.length == 8, "Expected 8 but got " SIZE_T_FORMAT, inputRecorder->memwriteContext.length);
	TestCase_assert(!memcmp(inputRecorder->memwriteContext.data, "\x00\x00\x00\x00\x00\x00\x00\x00", 8), "Expected 0000 00000000 0000 but got %02X%02X %02X%02X%02X%02X %02X%02X", ((unsigned char *) inputRecorder->memwriteContext.data)[0], ((unsigned char *) inputRecorder->memwriteContext.data)[1], ((unsigned char *) inputRecorder->memwriteContext.data)[2], ((unsigned char *) inputRecorder->memwriteContext.data)[3], ((unsigned char *) inputRecorder->memwriteContext.data)[4], ((unsigned char *) inputRecorder->memwriteContext.data)[5], ((unsigned char *) inputRecorder->memwriteContext.data)[6], ((unsigned char *) inputRecorder->memwriteContext.data)[7]);
	inputRecorder->dispose(inputRecorder);
	
	inputRecorder = InputRecorder_createWithMemwriteOutput(inputController, "abcd", 4);
	TestCase_assert(inputRecorder->memwriteContext.length == 12, "Expected 12 but got " SIZE_T_FORMAT, inputRecorder->memwriteContext.length);
	TestCase_assert(!memcmp(inputRecorder->memwriteContext.data, "\x00\x00\x04\x00\x00\x00""abcd\x00\x00", 12), "Expected 0000 04000000 61626364 0000 but got %02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X", ((unsigned char *) inputRecorder->memwriteContext.data)[0], ((unsigned char *) inputRecorder->memwriteContext.data)[1], ((unsigned char *) inputRecorder->memwriteContext.data)[2], ((unsigned char *) inputRecorder->memwriteContext.data)[3], ((unsigned char *) inputRecorder->memwriteContext.data)[4], ((unsigned char *) inputRecorder->memwriteContext.data)[5], ((unsigned char *) inputRecorder->memwriteContext.data)[6], ((unsigned char *) inputRecorder->memwriteContext.data)[7], ((unsigned char *) inputRecorder->memwriteContext.data)[8], ((unsigned char *) inputRecorder->memwriteContext.data)[9], ((unsigned char *) inputRecorder->memwriteContext.data)[10], ((unsigned char *) inputRecorder->memwriteContext.data)[11]);
	inputRecorder->dispose(inputRecorder);
	
	inputController->dispose(inputController);
	inputController = InputController_create(NULL, "a", "bc", NULL);
	
	inputRecorder = InputRecorder_createWithMemwriteOutput(inputController, NULL, 0);
	TestCase_assert(inputRecorder->memwriteContext.length == 13, "Expected 13 but got " SIZE_T_FORMAT, inputRecorder->memwriteContext.length);
	TestCase_assert(!memcmp(inputRecorder->memwriteContext.data, "\x00\x00\x00\x00\x00\x00\x02\x00""a\x00""bc\x00", 13), "Expected 0000 00000000 0200 6100 626300 but got %02X%02X %02X%02X%02X%02X %02X%02X %02X%02X %02X%02X%02X", ((unsigned char *) inputRecorder->memwriteContext.data)[0], ((unsigned char *) inputRecorder->memwriteContext.data)[1], ((unsigned char *) inputRecorder->memwriteContext.data)[2], ((unsigned char *) inputRecorder->memwriteContext.data)[3], ((unsigned char *) inputRecorder->memwriteContext.data)[4], ((unsigned char *) inputRecorder->memwriteContext.data)[5], ((unsigned char *) inputRecorder->memwriteContext.data)[6], ((unsigned char *) inputRecorder->memwriteContext.data)[7], ((unsigned char *) inputRecorder->memwriteContext.data)[8], ((unsigned char *) inputRecorder->memwriteContext.data)[9], ((unsigned char *) inputRecorder->memwriteContext.data)[10], ((unsigned char *) inputRecorder->memwriteContext.data)[11], ((unsigned char *) inputRecorder->memwriteContext.data)[12]);
	
	inputController->triggerAction(inputController, ATOM("a"));
	TestCase_assert(inputRecorder->memwriteContext.length == 19, "Expected 19 but got " SIZE_T_FORMAT, inputRecorder->memwriteContext.length);
	TestCase_assert(!memcmp(inputRecorder->memwriteContext.data + 13, "\x00\x00\x00\x00\x00\x00", 6), "Expected 00000000 0000 but got %02X%02X%02X%02X %02X%02X", ((unsigned char *) inputRecorder->memwriteContext.data)[13], ((unsigned char *) inputRecorder->memwriteContext.data)[14], ((unsigned char *) inputRecorder->memwriteContext.data)[15], ((unsigned char *) inputRecorder->memwriteContext.data)[16], ((unsigned char *) inputRecorder->memwriteContext.data)[17], ((unsigned char *) inputRecorder->memwriteContext.data)[18]);
	
	inputRecorder->nextFrame(inputRecorder);
	inputController->triggerAction(inputController, ATOM("bc"));
	TestCase_assert(inputRecorder->memwriteContext.length == 25, "Expected 25 but got " SIZE_T_FORMAT, inputRecorder->memwriteContext.length);
	TestCase_assert(!memcmp(inputRecorder->memwriteContext.data + 19, "\x01\x00\x00\x00\x01\x00", 6), "Expected 01000000 0100 but got %02X%02X%02X%02X %02X%02X", ((unsigned char *) inputRecorder->memwriteContext.data)[19], ((unsigned char *) inputRecorder->memwriteContext.data)[20], ((unsigned char *) inputRecorder->memwriteContext.data)[21], ((unsigned char *) inputRecorder->memwriteContext.data)[22], ((unsigned char *) inputRecorder->memwriteContext.data)[23], ((unsigned char *) inputRecorder->memwriteContext.data)[24]);
	
	inputRecorder->nextFrame(inputRecorder);
	inputRecorder->nextFrame(inputRecorder);
	inputRecorder->nextFrame(inputRecorder);
	inputController->releaseAction(inputController, ATOM("bc"));
	TestCase_assert(inputRecorder->memwriteContext.length == 31, "Expected 31 but got " SIZE_T_FORMAT, inputRecorder->memwriteContext.length);
	TestCase_assert(!memcmp(inputRecorder->memwriteContext.data + 25, "\x03\x00\x00\x00\x01\x00", 6), "Expected 03000000 0100 but got %02X%02X%02X%02X %02X%02X", ((unsigned char *) inputRecorder->memwriteContext.data)[25], ((unsigned char *) inputRecorder->memwriteContext.data)[26], ((unsigned char *) inputRecorder->memwriteContext.data)[27], ((unsigned char *) inputRecorder->memwriteContext.data)[28], ((unsigned char *) inputRecorder->memwriteContext.data)[29], ((unsigned char *) inputRecorder->memwriteContext.data)[30]);
	
	inputRecorder->dispose(inputRecorder);
	inputController->dispose(inputController);
}

static void testFileRecording() {
	InputRecorder * inputRecorder;
	InputController * inputController;
	const char * tempFilePath;
	int tempFD;
	unsigned char * fileContents;
	size_t fileLength;
	
	inputController = InputController_create(NULL, NULL);
	tempFilePath = temporaryFilePath("tmpXXXXXX", &tempFD);
	close(tempFD);
	
	inputRecorder = InputRecorder_createWithFileOutput(inputController, NULL, 0, tempFilePath);
	fileContents = readFileSimple(tempFilePath, &fileLength);
	TestCase_assert(fileContents != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fileLength == 8, "Expected 8 but got " SIZE_T_FORMAT, fileLength);
	TestCase_assert(!memcmp(fileContents, "\x00\x00\x00\x00\x00\x00\x00\x00", 8), "Expected 0000 00000000 0000 but got %02X%02X %02X%02X%02X%02X %02X%02X", fileContents[0], fileContents[1], fileContents[2], fileContents[3], fileContents[4], fileContents[5], fileContents[6], fileContents[7]);
	free(fileContents);
	inputRecorder->dispose(inputRecorder);
	
	inputRecorder = InputRecorder_createWithFileOutput(inputController, "abcd", 4, tempFilePath);
	fileContents = readFileSimple(tempFilePath, &fileLength);
	TestCase_assert(fileContents != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fileLength == 12, "Expected 12 but got " SIZE_T_FORMAT, fileLength);
	TestCase_assert(!memcmp(fileContents, "\x00\x00\x04\x00\x00\x00""abcd\x00\x00", 12), "Expected 0000 04000000 61626364 0000 but got %02X%02X %02X%02X%02X%02X %02X%02X%02X%02X %02X%02X", fileContents[0], fileContents[1], fileContents[2], fileContents[3], fileContents[4], fileContents[5], fileContents[6], fileContents[7], fileContents[8], fileContents[9], fileContents[10], fileContents[11]);
	free(fileContents);
	inputRecorder->dispose(inputRecorder);
	
	inputController->dispose(inputController);
	inputController = InputController_create(NULL, "a", "bc", NULL);
	
	inputRecorder = InputRecorder_createWithFileOutput(inputController, NULL, 0, tempFilePath);
	fileContents = readFileSimple(tempFilePath, &fileLength);
	TestCase_assert(fileContents != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fileLength == 13, "Expected 13 but got " SIZE_T_FORMAT, fileLength);
	TestCase_assert(!memcmp(fileContents, "\x00\x00\x00\x00\x00\x00\x02\x00""a\x00""bc\x00", 13), "Expected 0000 00000000 0200 6100 626300 but got %02X%02X %02X%02X%02X%02X %02X%02X %02X%02X %02X%02X%02X", fileContents[0], fileContents[1], fileContents[2], fileContents[3], fileContents[4], fileContents[5], fileContents[6], fileContents[7], fileContents[8], fileContents[9], fileContents[10], fileContents[11], fileContents[12]);
	free(fileContents);
	
	inputController->triggerAction(inputController, ATOM("a"));
	fileContents = readFileSimple(tempFilePath, &fileLength);
	TestCase_assert(fileContents != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fileLength == 19, "Expected 19 but got " SIZE_T_FORMAT, fileLength);
	TestCase_assert(!memcmp(fileContents + 13, "\x00\x00\x00\x00\x00\x00", 6), "Expected 00000000 0000 but got %02X%02X%02X%02X %02X%02X", fileContents[13], fileContents[14], fileContents[15], fileContents[16], fileContents[17], fileContents[18]);
	free(fileContents);
	
	inputRecorder->nextFrame(inputRecorder);
	inputController->triggerAction(inputController, ATOM("bc"));
	fileContents = readFileSimple(tempFilePath, &fileLength);
	TestCase_assert(fileContents != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fileLength == 25, "Expected 25 but got " SIZE_T_FORMAT, fileLength);
	TestCase_assert(!memcmp(fileContents + 19, "\x01\x00\x00\x00\x01\x00", 6), "Expected 01000000 0100 but got %02X%02X%02X%02X %02X%02X", fileContents[19], fileContents[20], fileContents[21], fileContents[22], fileContents[23], fileContents[24]);
	free(fileContents);
	
	inputRecorder->nextFrame(inputRecorder);
	inputRecorder->nextFrame(inputRecorder);
	inputRecorder->nextFrame(inputRecorder);
	inputController->releaseAction(inputController, ATOM("bc"));
	fileContents = readFileSimple(tempFilePath, &fileLength);
	TestCase_assert(fileContents != NULL, "Expected non-NULL but got NULL");
	TestCase_assert(fileLength == 31, "Expected 31 but got " SIZE_T_FORMAT, fileLength);
	TestCase_assert(!memcmp(fileContents + 25, "\x03\x00\x00\x00\x01\x00", 6), "Expected 03000000 0100 but got %02X%02X%02X%02X %02X%02X", fileContents[25], fileContents[26], fileContents[27], fileContents[28], fileContents[29], fileContents[30]);
	free(fileContents);
	
	inputRecorder->dispose(inputRecorder);
	inputController->dispose(inputController);
	unlink(tempFilePath);
}

TEST_SUITE(InputRecorderTest,
           testInit,
           testMemwriteRecording,
           testFileRecording)
