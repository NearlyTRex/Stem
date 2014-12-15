/*
  Copyright (c) 2014 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com
*/

#include "inputcontroller/InputRecorder.h"
#include "inputcontroller/InputSession.h"
#include "utilities/IOUtilities.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

InputRecorder * InputRecorder_createWithFileOutput(InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize, const char * filePath) {
	stemobject_create_implementation(InputRecorder, initWithFileOutput, inputController, replayStartupData, replayStartupDataSize, filePath)
}

InputRecorder * InputRecorder_createWithMemwriteOutput(InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize) {
	stemobject_create_implementation(InputRecorder, initWithMemwriteOutput, inputController, replayStartupData, replayStartupDataSize)
}

static void writeData(InputRecorder * self, unsigned int size, const void * data) {
	if (self->outputFile != NULL) {
		fwrite(data, 1, size, self->outputFile);
	} else if (self->memwriteContext.data != NULL || self->memwriteContext.realloc) {
		memwrite(&self->memwriteContext, size, data);
	}
}

static bool action(Atom eventID, void * eventData, void * context) {
	InputRecorder * self = context;
	unsigned int actionIndex;
	uint16_t uint16;
	uint32_t uint32;
	
	for (actionIndex = 0; actionIndex < self->inputController->actionCount; actionIndex++) {
		if (self->inputController->actions[actionIndex].actionID == eventData) {
			break;
		}
	}
	if (actionIndex >= self->inputController->actionCount) {
		fprintf(stderr, "Warning: Unrecognized actionID \"%s\" %s in InputRecorder\n", (const char *) eventData, eventID == ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN) ? "triggered" : "released");
		return false;
	}
	
	uint32 = swapLittleEndian32(self->frameIndex - self->lastFrameIndex);
	writeData(self, 4, &uint32);
	uint16 = swapLittleEndian16(actionIndex);
	writeData(self, 2, &uint16);
	self->lastFrameIndex = self->frameIndex;
	return true;
}

static void sharedInit(InputRecorder * self, InputController * inputController) {
	self->inputController = inputController;
	self->frameIndex = 0;
	self->lastFrameIndex = 0;
	self->dispose = InputRecorder_dispose;
	
	if (self->inputController != NULL) {
		EventDispatcher_registerForEvent(self->inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), action, self);
		EventDispatcher_registerForEvent(self->inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), action, self);
	}
}

static void writeHeader(InputRecorder * self, const void * replayStartupData, uint32_t replayStartupDataSize) {
	uint16_t uint16;
	uint32_t uint32;
	
	uint16 = swapLittleEndian16(INPUT_SESSION_FORMAT_VERSION);
	writeData(self, 2, &uint16);
	uint32 = swapLittleEndian32(replayStartupDataSize);
	writeData(self, 4, &uint32);
	writeData(self, replayStartupDataSize, replayStartupData);
	
	if (self->inputController == NULL) {
		writeData(self, 2, NULL);
		
	} else {
		unsigned int actionIndex;
		
		uint16 = swapLittleEndian16(self->inputController->actionCount);
		writeData(self, 2, &uint16);
		for (actionIndex = 0; actionIndex < self->inputController->actionCount; actionIndex++) {
			writeData(self, strlen(self->inputController->actions[actionIndex].actionID) + 1, self->inputController->actions[actionIndex].actionID);
		}
	}
}

bool InputRecorder_initWithFileOutput(InputRecorder * self, InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize, const char * filePath) {
	call_super(init, self);
	sharedInit(self, inputController);
	self->memwriteContext.data = NULL;
	self->memwriteContext.realloc = false;
	self->outputFile = fopen(filePath, "wb");
	if (self->outputFile == NULL) {
		self->dispose(self);
		return false;
	}
	setvbuf(self->outputFile, NULL, _IONBF, 0);
	writeHeader(self, replayStartupData, replayStartupDataSize);
	return true;
}

bool InputRecorder_initWithMemwriteOutput(InputRecorder * self, InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize) {
	call_super(init, self);
	sharedInit(self, inputController);
	self->outputFile = NULL;
	self->memwriteContext = memwriteContextInit(NULL, 0, 0, true);
	writeHeader(self, replayStartupData, replayStartupDataSize);
	return true;
}

void InputRecorder_dispose(InputRecorder * self) {
	call_super(dispose, self);
	if (self->inputController != NULL) {
		EventDispatcher_unregisterForEvent(self->inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), action, self);
		EventDispatcher_unregisterForEvent(self->inputController->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), action, self);
	}
	if (self->outputFile == NULL) {
		free(self->memwriteContext.data);
	} else {
		fclose(self->outputFile);
	}
}

void InputRecorder_nextFrame(InputRecorder * self) {
	self->frameIndex++;
}
