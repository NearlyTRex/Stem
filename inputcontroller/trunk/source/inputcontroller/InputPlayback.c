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

#define SUPERCLASS StemObject

InputPlayback * InputPlayback_create(InputController * inputController, InputSession * inputSession) {
	stemobject_create_implementation(InputPlayback, init, inputController, inputSession)
}

void InputPlayback_init(InputPlayback * self, InputController * inputController, InputSession * inputSession) {
	call_super(init, self);
	self->dispose = InputPlayback_dispose;
	self->step = InputPlayback_step;
	self->rewind = InputPlayback_rewind;
	self->inputController = inputController;
	self->inputSession = inputSession;
	self->eventDispatcher = EventDispatcher_create(self);
	self->frameIndex = 0;
}

void InputPlayback_dispose(InputPlayback * self) {
	self->eventDispatcher->dispose(self->eventDispatcher);
	call_super(dispose, self);
}

void InputPlayback_step(InputPlayback * self) {
}

void InputPlayback_rewind(InputPlayback * self) {
}
