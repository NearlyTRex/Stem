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

#ifndef __AL_AUDIO_BUFFER_H__
#define __AL_AUDIO_BUFFER_H__

typedef struct ALAudioBuffer ALAudioBuffer;

#include "alaudio/ALIncludes.h"
#include "pcmaudio/PCMAudio.h"
#include "stemobject/StemObject.h"
#include <stdlib.h>

#define ALAudioBuffer_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	ALuint buffer; \
	void (* bufferAudioData)(self_type * self, ALenum format, void * data, size_t length, unsigned int frequency); \
	void (* bufferPCMAudio)(self_type * self, PCMAudio * audio);

stemobject_struct_definition(ALAudioBuffer)

ALAudioBuffer * ALAudioBuffer_create();
void ALAudioBuffer_init(ALAudioBuffer * self);
void ALAudioBuffer_dispose(ALAudioBuffer * self);

void ALAudioBuffer_bufferAudioData(ALAudioBuffer * self, ALenum format, void * data, size_t length, unsigned int frequency);
void ALAudioBuffer_bufferPCMAudio(ALAudioBuffer * self, PCMAudio * pcmAudio);

#endif
