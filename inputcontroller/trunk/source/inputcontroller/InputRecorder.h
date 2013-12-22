/*
  Copyright (c) 2013 Alex Diener
  
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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __InputRecorder_H__
#define __InputRecorder_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct InputRecorder InputRecorder;

#include "stemobject/StemObject.h"
#include "inputcontroller/InputController.h"
#include "utilities/IOUtilities.h"
#include <stdio.h>
#include <stdint.h>

#define InputRecorder_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	InputController * inputController; \
	unsigned int frameIndex; \
	unsigned int lastFrameIndex; \
	FILE * outputFile; \
	struct memwriteContext memwriteContext; \
	\
	void (* nextFrame)(self_type * self);

stemobject_struct_definition(InputRecorder)

InputRecorder * InputRecorder_createWithFileOutput(InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize, const char * filePath);
InputRecorder * InputRecorder_createWithMemwriteOutput(InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize);
void InputRecorder_initWithFileOutput(InputRecorder * self, InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize, const char * filePath);
void InputRecorder_initWithMemwriteOutput(InputRecorder * self, InputController * inputController, const void * replayStartupData, uint32_t replayStartupDataSize);
void InputRecorder_dispose(InputRecorder * self);
void InputRecorder_nextFrame(InputRecorder * self);

#ifdef __cplusplus
}
#endif
#endif
