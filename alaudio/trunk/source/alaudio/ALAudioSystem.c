/*
  Copyright (c) 2012 Alex Diener
  
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

#include "alaudio/ALAudioSystem.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

ALAudioSystem * ALAudioSystem_create() {
	stemobject_create_implementation(ALAudioSystem, init)
}

void ALAudioSystem_init(ALAudioSystem * self) {
	call_super(init, self);
	
	self->device = alcOpenDevice(NULL);
	self->context = alcCreateContext(self->device, NULL);
	alcMakeContextCurrent(self->context);
	
	self->dispose = ALAudioSystem_dispose;
	self->makeCurrentContext = ALAudioSystem_makeCurrentContext;
}

void ALAudioSystem_dispose(ALAudioSystem * self) {
	alcDestroyContext(self->context);
	alcCloseDevice(self->device);
	call_super(dispose, self);
}

void ALAudioSystem_makeCurrentContext(ALAudioSystem * self) {
	alcMakeContextCurrent(self->context);
}
