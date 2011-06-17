#ifndef __PCM_AUDIO_H__
#define __PCM_AUDIO_H__

#include "stemobject/StemObject.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct PCMAudio PCMAudio;

#define PCMAudio_structContents \
	StemObject_structContents \
	\
	unsigned int bytesPerSample; \
	unsigned int channelCount; \
	unsigned int sampleRate; \
	size_t sampleCount; \
	void * samples;

struct PCMAudio {
	PCMAudio_structContents
};

// If copySamples is false, PCMAudio takes ownership of the samples pointer and the caller must not free it.
// If copySamples is true, the caller retains ownership of the samples pointer and must free it.
PCMAudio * PCMAudio_create(unsigned int bytesPerSample,
                           unsigned int channelCount,
                           unsigned int sampleRate,
                           size_t sampleCount,
                           void * samples,
                           bool copySamples);
void PCMAudio_init(compat_type(PCMAudio *) selfPtr,
                   unsigned int bytesPerSample,
                   unsigned int channelCount,
                   unsigned int sampleRate,
                   size_t sampleCount,
                   void * samples,
                   bool copySamples);
void PCMAudio_dispose(compat_type(PCMAudio *) selfPtr);

#endif
