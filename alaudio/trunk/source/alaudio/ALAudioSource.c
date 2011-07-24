/*
  Copyright (c) 2011 Alex Diener
  
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

#include "alaudio/ALAudioSource.h"

ALAudioSource * ALAudioSource_create() {
	stemobject_create_implementation(ALAudioSource, init)
}

void ALAudioSource_init(compat_type(ALAudioSource *) selfPtr) {
	ALAudioSource * self = selfPtr;
	
	StemObject_init(self);
	alGenSources(1, &self->source);
	self->dispose = ALAudioSource_dispose;
	self->load = ALAudioSource_load;
	self->play = ALAudioSource_play;
	self->stop = ALAudioSource_stop;
}

void ALAudioSource_dispose(compat_type(ALAudioSource *) selfPtr) {
	ALAudioSource * self = selfPtr;
	
	alDeleteSources(1, &self->source);
	StemObject_dispose(self);
}

void ALAudioSource_load(compat_type(ALAudioSource *) selfPtr, ALAudioBuffer * buffer) {
	ALAudioSource * self = selfPtr;
	
	alSourcei(self->source, AL_BUFFER, buffer->buffer);
}

void ALAudioSource_play(compat_type(ALAudioSource *) selfPtr) {
	ALAudioSource * self = selfPtr;
	
	alSourcePlay(self->source);
}

void ALAudioSource_stop(compat_type(ALAudioSource *) selfPtr) {
	ALAudioSource * self = selfPtr;
	
	alSourceStop(self->source);
	alSourceRewind(self->source);
}

void ALAudioSource_pause(compat_type(ALAudioSource *) selfPtr) {
	ALAudioSource * self = selfPtr;
	
	alSourcePause(self->source);
}
