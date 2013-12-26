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

#include "pcmaudio/PCMAudio.h"
#include <stdlib.h>
#include <string.h>

#define SUPERCLASS StemObject

PCMAudio * PCMAudio_create(unsigned int bytesPerSample,
                           unsigned int channelCount,
                           unsigned int sampleRate,
                           size_t sampleCount,
                           void * samples,
                           bool copySamples) {
	stemobject_create_implementation(PCMAudio, init, bytesPerSample, channelCount, sampleRate, sampleCount, samples, copySamples)
}

bool PCMAudio_init(PCMAudio * self,
                   unsigned int bytesPerSample,
                   unsigned int channelCount,
                   unsigned int sampleRate,
                   size_t sampleCount,
                   void * samples,
                   bool copySamples) {
	call_super(init, self);
	
	self->bytesPerSample = bytesPerSample;
	self->channelCount = channelCount;
	self->sampleRate = sampleRate;
	self->sampleCount = sampleCount;
	
	if (copySamples) {
		self->samples = malloc(bytesPerSample * channelCount * sampleCount);
		memcpy(self->samples, samples, bytesPerSample * channelCount * sampleCount);
		
	} else {
		self->samples = samples;
	}
	
	self->dispose = PCMAudio_dispose;
	return true;
}

void PCMAudio_dispose(PCMAudio * self) {
	free(self->samples);
	call_super(dispose, self);
}
