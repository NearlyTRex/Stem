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

#ifndef __InputSession_H__
#define __InputSession_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct InputSession InputSession;

#include <stdlib.h>
#include "utilities/Atom.h"

#define INPUT_SESSION_FORMAT_VERSION 0

enum InputSessionError {
	INPUT_SESSION_ERROR_FILE_NOT_FOUND,
	INPUT_SESSION_ERROR_FORMAT_TOO_NEW,
	INPUT_SESSION_ERROR_PREMATURE_EOF,
	INPUT_SESSION_ERROR_INVALID_ACTION_INDEX
};

struct InputSessionEvent {
	unsigned int frameDelta;
	unsigned int actionIndex;
};

struct InputSession {
	size_t replayStartupDataSize;
	void * replayStartupData;
	unsigned int actionCount;
	Atom * actions;
	unsigned int eventCount;
	struct InputSessionEvent * events;
};

InputSession * InputSession_loadFile(const char * filePath, enum InputSessionError * outError);
InputSession * InputSession_loadData(const void * data, size_t length, enum InputSessionError * outError);
void InputSession_dispose(InputSession * inputSession);

#ifdef __cplusplus
}
#endif
#endif
