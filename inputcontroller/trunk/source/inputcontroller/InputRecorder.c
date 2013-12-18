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

#include "inputcontroller/InputRecorder.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

InputRecorder * InputRecorder_createWithFileOutput(InputController * inputController, void * replayStartupData, size_t replayStartupDataSize, const char * filePath) {
	stemobject_create_implementation(InputRecorder, initWithFileOutput, inputController, replayStartupData, replayStartupDataSize, filePath)
}

InputRecorder * InputRecorder_createWithMemwriteOutput(InputController * inputController, void * replayStartupData, size_t replayStartupDataSize) {
	stemobject_create_implementation(InputRecorder, initWithMemwriteOutput, inputController, replayStartupData, replayStartupDataSize)
}

void InputRecorder_initWithFileOutput(InputRecorder * self, InputController * inputController, void * replayStartupData, size_t replayStartupDataSize, const char * filePath) {
	call_super(init, self);
}

void InputRecorder_initWithMemwriteOutput(InputRecorder * self, InputController * inputController, void * replayStartupData, size_t replayStartupDataSize) {
	call_super(init, self);
}

void InputRecorder_dispose(InputRecorder * self) {
	call_super(dispose, self);
}

void InputRecorder_nextFrame(InputRecorder * self) {
}

void InputRecorder_skipFrames(InputRecorder * self, unsigned int frameCount) {
}
