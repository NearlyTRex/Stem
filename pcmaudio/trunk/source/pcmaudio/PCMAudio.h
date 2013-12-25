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

#ifndef __PCM_AUDIO_H__
#define __PCM_AUDIO_H__

#include "stemobject/StemObject.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct PCMAudio PCMAudio;

#define PCMAudio_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	unsigned int bytesPerSample; \
	unsigned int channelCount; \
	unsigned int sampleRate; \
	size_t sampleCount; \
	void * samples;

stemobject_struct_definition(PCMAudio)

// If copySamples is false, PCMAudio takes ownership of the samples pointer and the caller must not free it.
// If copySamples is true, the caller retains ownership of the samples pointer and must free it.
PCMAudio * PCMAudio_create(unsigned int bytesPerSample,
                           unsigned int channelCount,
                           unsigned int sampleRate,
                           size_t sampleCount,
                           void * samples,
                           bool copySamples);
bool PCMAudio_init(PCMAudio * self,
                   unsigned int bytesPerSample,
                   unsigned int channelCount,
                   unsigned int sampleRate,
                   size_t sampleCount,
                   void * samples,
                   bool copySamples);
void PCMAudio_dispose(PCMAudio * self);

#endif
