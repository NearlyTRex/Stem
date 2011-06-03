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

#include "alaudio/ALAudioBuffer.h"

ALAudioBuffer * ALAudioBuffer_create() {
	stemobject_create_implementation(ALAudioBuffer, init)
}

void ALAudioBuffer_init(compat_type(ALAudioBuffer *) selfPtr) {
	ALAudioBuffer * self = selfPtr;
	
	StemObject_init(self);
	alGenBuffers(1, &self->buffer);
	self->dispose = ALAudioBuffer_dispose;
	self->bufferAudioData = ALAudioBuffer_bufferAudioData;
}

void ALAudioBuffer_dispose(compat_type(ALAudioBuffer *) selfPtr) {
	ALAudioBuffer * self = selfPtr;
	
	alDeleteBuffers(1, &self->buffer);
	StemObject_dispose(self);
}

void ALAudioBuffer_bufferAudioData(compat_type(ALAudioBuffer *) selfPtr, ALenum format, void * data, size_t length, unsigned int frequency) {
	ALAudioBuffer * self = selfPtr;
	
	alBufferData(self->buffer, format, data, length, frequency);
}
