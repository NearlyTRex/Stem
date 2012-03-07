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

#include "alaudio/ALAudioSource.h"

#define SUPERCLASS StemObject

ALAudioSource * ALAudioSource_create() {
	stemobject_create_implementation(ALAudioSource, init)
}

void ALAudioSource_init(ALAudioSource * self) {
	call_super(init, self);
	alGenSources(1, &self->source);
	self->dispose = ALAudioSource_dispose;
	self->load = ALAudioSource_load;
	self->play = ALAudioSource_play;
	self->stop = ALAudioSource_stop;
	self->pause = ALAudioSource_pause;
}

void ALAudioSource_dispose(ALAudioSource * self) {
	alDeleteSources(1, &self->source);
	call_super(dispose, self);
}

void ALAudioSource_load(ALAudioSource * self, ALAudioBuffer * buffer) {
	alSourcei(self->source, AL_BUFFER, buffer->buffer);
}

void ALAudioSource_play(ALAudioSource * self) {
	alSourcePlay(self->source);
}

void ALAudioSource_stop(ALAudioSource * self) {
	alSourceStop(self->source);
	alSourceRewind(self->source);
}

void ALAudioSource_pause(ALAudioSource * self) {
	alSourcePause(self->source);
}
