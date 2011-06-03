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

#ifndef __AL_AUDIO_BUFFER_H__
#define __AL_AUDIO_BUFFER_H__

typedef struct ALAudioBuffer ALAudioBuffer;

#include "stemobject/StemObject.h"
#include "alaudio/ALIncludes.h"
#include <stdlib.h>

#define ALAudioBuffer_structContents \
	StemObject_structContents \
	\
	ALuint buffer; \
	void (* bufferAudioData)(compat_type(ALAudioBuffer *) self, ALenum format, void * data, size_t length, unsigned int frequency);

struct ALAudioBuffer {
	ALAudioBuffer_structContents
};

ALAudioBuffer * ALAudioBuffer_create();
void ALAudioBuffer_init(compat_type(ALAudioBuffer *) selfPtr);
void ALAudioBuffer_dispose(compat_type(ALAudioBuffer *) selfPtr);

void ALAudioBuffer_bufferAudioData(compat_type(ALAudioBuffer *) selfPtr, ALenum format, void * data, size_t length, unsigned int frequency);

#endif
