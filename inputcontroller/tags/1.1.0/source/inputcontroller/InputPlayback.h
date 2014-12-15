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

#ifndef __InputPlayback_H__
#define __InputPlayback_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct InputPlayback InputPlayback;

#include "stemobject/StemObject.h"
#include "inputcontroller/InputController.h"
#include "inputcontroller/InputSession.h"
#include <stdbool.h>

// eventData: NULL
#define INPUT_PLAYBACK_EVENT_PLAYBACK_COMPLETE "INPUT_PLAYBACK_EVENT_PLAYBACK_COMPLETE"

#define InputPlayback_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	InputController * inputController; \
	InputSession * inputSession; \
	EventDispatcher * eventDispatcher; \
	unsigned int frameIndex; \
	unsigned int lastFrameIndex; \
	unsigned int eventIndex; \
	bool * actionsTriggered;

stemobject_struct_definition(InputPlayback)

InputPlayback * InputPlayback_create(InputController * inputController, InputSession * inputSession);
bool InputPlayback_init(InputPlayback * self, InputController * inputController, InputSession * inputSession);
void InputPlayback_dispose(InputPlayback * self);
void InputPlayback_step(InputPlayback * self);
void InputPlayback_rewind(InputPlayback * self);

#ifdef __cplusplus
}
#endif
#endif
