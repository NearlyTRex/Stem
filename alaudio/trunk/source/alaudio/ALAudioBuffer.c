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

#include "alaudio/ALAudioBuffer.h"

#define SUPERCLASS StemObject

ALAudioBuffer * ALAudioBuffer_create() {
	stemobject_create_implementation(ALAudioBuffer, init)
}

void ALAudioBuffer_init(ALAudioBuffer * self) {
	call_super(init, self);
	alGenBuffers(1, &self->buffer);
	self->dispose = ALAudioBuffer_dispose;
	self->bufferAudioData = ALAudioBuffer_bufferAudioData;
	self->bufferPCMAudio = ALAudioBuffer_bufferPCMAudio;
}

void ALAudioBuffer_dispose(ALAudioBuffer * self) {
	alDeleteBuffers(1, &self->buffer);
	call_super(dispose, self);
}

void ALAudioBuffer_bufferAudioData(ALAudioBuffer * self, ALenum format, void * data, size_t length, unsigned int frequency) {
	alBufferData(self->buffer, format, data, length, frequency);
}

void ALAudioBuffer_bufferPCMAudio(ALAudioBuffer * self, PCMAudio * pcmAudio) {
	self->bufferAudioData(self,
	                      pcmAudio->channelCount == 1 ? pcmAudio->bytesPerSample == 1 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16 : pcmAudio->bytesPerSample == 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16,
	                      pcmAudio->samples,
	                      pcmAudio->sampleCount * pcmAudio->bytesPerSample * pcmAudio->channelCount,
	                      pcmAudio->sampleRate);
}
