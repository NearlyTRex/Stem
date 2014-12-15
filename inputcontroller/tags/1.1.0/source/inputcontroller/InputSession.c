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

#include "inputcontroller/InputSession.h"
#include "utilities/IOUtilities.h"
#include <stdint.h>

InputSession * InputSession_loadFile(const char * filePath, enum InputSessionError * outError) {
	InputSession * self;
	void * fileData;
	size_t length;
	
	fileData = readFileSimple(filePath, &length);
	if (fileData == NULL) {
		if (outError != NULL) {
			*outError = INPUT_SESSION_ERROR_FILE_NOT_FOUND;
		}
		return NULL;
	}
	self = InputSession_loadData(fileData, length, outError);
	free(fileData);
	return self;
}

InputSession * InputSession_loadData(const void * data, size_t length, enum InputSessionError * outError) {
	InputSession * self;
	struct memreadContext context;
	uint16_t uint16;
	uint32_t uint32;
	unsigned int formatVersion, actionIndex, eventIndex;
	size_t actionStartPosition;
	
	context = memreadContextInit(data, length);
	if (!memread(&context, 2, &uint16)) {
		if (outError != NULL) {
			*outError = INPUT_SESSION_ERROR_PREMATURE_EOF;
		}
		return NULL;
	}
	formatVersion = swapLittleEndian16(uint16);
	if (formatVersion > INPUT_SESSION_FORMAT_VERSION) {
		if (outError != NULL) {
			*outError = INPUT_SESSION_ERROR_FORMAT_TOO_NEW;
		}
		return NULL;
	}
	
	if (!memread(&context, 4, &uint32)) {
		if (outError != NULL) {
			*outError = INPUT_SESSION_ERROR_PREMATURE_EOF;
		}
		return NULL;
	}
	self = malloc(sizeof(InputSession));
	self->replayStartupDataSize = swapLittleEndian32(uint32);
	
	if (self->replayStartupDataSize == 0) {
		self->replayStartupData = NULL;
	} else {
		self->replayStartupData = malloc(self->replayStartupDataSize);
		if (!memread(&context, self->replayStartupDataSize, self->replayStartupData)) {
			if (outError != NULL) {
				*outError = INPUT_SESSION_ERROR_PREMATURE_EOF;
			}
			free(self->replayStartupData);
			free(self);
			return NULL;
		}
	}
	
	if (!memread(&context, 2, &uint16)) {
		if (outError != NULL) {
			*outError = INPUT_SESSION_ERROR_PREMATURE_EOF;
		}
		free(self->replayStartupData);
		free(self);
		return NULL;
	}
	self->actionCount = swapLittleEndian16(uint16);
	if (self->actionCount == 0) {
		self->actions = NULL;
	} else {
		self->actions = malloc(sizeof(Atom) * self->actionCount);
		for (actionIndex = 0; actionIndex < self->actionCount; actionIndex++) {
			actionStartPosition = context.position;
			while (context.position < context.length && ((char *) context.data)[context.position] != '\x00') {
				context.position++;
			}
			context.position++;
			if (context.position > context.length) {
				if (outError != NULL) {
					*outError = INPUT_SESSION_ERROR_PREMATURE_EOF;
				}
				free(self->replayStartupData);
				free(self->actions);
				free(self);
				return NULL;
			}
			self->actions[actionIndex] = Atom_fromString(context.data + actionStartPosition);
		}
	}
	
	self->eventCount = (context.length - context.position) / 6;
	self->events = malloc(sizeof(struct InputSessionEvent) * self->eventCount);
	for (eventIndex = 0; eventIndex < self->eventCount; eventIndex++) {
		memread(&context, 4, &uint32);
		self->events[eventIndex].frameDelta = swapLittleEndian32(uint32);
		memread(&context, 2, &uint16);
		self->events[eventIndex].actionIndex = swapLittleEndian16(uint16);
		if (self->events[eventIndex].actionIndex >= self->actionCount) {
			if (outError != NULL) {
				*outError = INPUT_SESSION_ERROR_INVALID_ACTION_INDEX;
			}
			free(self->replayStartupData);
			free(self->actions);
			free(self->events);
			free(self);
			return NULL;
		}
	}
	
	return self;
}

void InputSession_dispose(InputSession * inputSession) {
	free(inputSession->replayStartupData);
	free(inputSession->actions);
	free(inputSession->events);
	free(inputSession);
}
