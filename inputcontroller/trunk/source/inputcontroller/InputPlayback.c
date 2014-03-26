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

#include "inputcontroller/InputPlayback.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

InputPlayback * InputPlayback_create(InputController * inputController, InputSession * inputSession) {
	stemobject_create_implementation(InputPlayback, init, inputController, inputSession)
}

bool InputPlayback_init(InputPlayback * self, InputController * inputController, InputSession * inputSession) {
	call_super(init, self);
	self->dispose = InputPlayback_dispose;
	self->step = InputPlayback_step;
	self->rewind = InputPlayback_rewind;
	self->inputController = inputController;
	self->inputSession = inputSession;
	self->eventDispatcher = EventDispatcher_create(self);
	self->frameIndex = 0;
	self->lastFrameIndex = 0;
	if (inputSession == NULL || inputSession->actionCount == 0) {
		self->actionsTriggered = NULL;
	} else {
		self->actionsTriggered = calloc(sizeof(bool), inputSession->actionCount);
	}
	self->eventIndex = 0;
	return true;
}

void InputPlayback_dispose(InputPlayback * self) {
	self->eventDispatcher->dispose(self->eventDispatcher);
	free(self->actionsTriggered);
	call_super(dispose, self);
}

void InputPlayback_step(InputPlayback * self) {
	while (self->eventIndex < self->inputSession->eventCount && self->frameIndex - self->lastFrameIndex >= self->inputSession->events[self->eventIndex].frameDelta) {
		if (self->actionsTriggered[self->inputSession->events[self->eventIndex].actionIndex]) {
			self->inputController->releaseAction(self->inputController, self->inputSession->actions[self->inputSession->events[self->eventIndex].actionIndex]);
			self->actionsTriggered[self->inputSession->events[self->eventIndex].actionIndex] = false;
			
		} else {
			self->inputController->triggerAction(self->inputController, self->inputSession->actions[self->inputSession->events[self->eventIndex].actionIndex]);
			self->actionsTriggered[self->inputSession->events[self->eventIndex].actionIndex] = true;
		}
		self->lastFrameIndex = self->frameIndex;
		self->eventIndex++;
		if (self->eventIndex >= self->inputSession->eventCount) {
			self->eventDispatcher->dispatchEvent(self->eventDispatcher, ATOM(INPUT_PLAYBACK_EVENT_PLAYBACK_COMPLETE), NULL);
		}
	}
	self->frameIndex++;
}

void InputPlayback_rewind(InputPlayback * self) {
	self->frameIndex = 0;
	self->lastFrameIndex = 0;
	self->eventIndex = 0;
	memset(self->actionsTriggered, 0x00, sizeof(bool) * self->inputSession->actionCount);
}
