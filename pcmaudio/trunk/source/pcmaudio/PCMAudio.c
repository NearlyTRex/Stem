#include "pcmaudio/PCMAudio.h"
#include <stdlib.h>
#include <string.h>

PCMAudio * PCMAudio_create(unsigned int bytesPerSample,
                           unsigned int channelCount,
                           unsigned int sampleRate,
                           unsigned int sampleCount,
                           void * samples,
                           bool copySamples) {
	stemobject_create_implementation(PCMAudio, init, bytesPerSample, channelCount, sampleRate, sampleCount, samples, copySamples)
}

void PCMAudio_init(compat_type(PCMAudio *) selfPtr,
                   unsigned int bytesPerSample,
                   unsigned int channelCount,
                   unsigned int sampleRate,
                   unsigned int sampleCount,
                   void * samples,
                   bool copySamples) {
	PCMAudio * self = selfPtr;
	
	StemObject_init(self);
	
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
}

void PCMAudio_dispose(compat_type(PCMAudio *) selfPtr) {
	PCMAudio * self = selfPtr;
	
	free(self->samples);
	StemObject_dispose(self);
}
