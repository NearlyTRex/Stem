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

#ifndef __AL_AUDIO_SOURCE_H__
#define __AL_AUDIO_SOURCE_H__

typedef struct ALAudioSource ALAudioSource;

#include "stemobject/StemObject.h"
#include "alaudio/ALAudioBuffer.h"

#define ALAudioSource_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	ALuint source; \
	void (* load)(self_type * self, ALAudioBuffer * buffer); \
	void (* play)(self_type * self); \
	void (* stop)(self_type * self); \
	void (* pause)(self_type * self);

stemobject_struct_definition(ALAudioSource)

ALAudioSource * ALAudioSource_create();
void ALAudioSource_init(ALAudioSource * self);
void ALAudioSource_dispose(ALAudioSource * self);

void ALAudioSource_load(ALAudioSource * self, ALAudioBuffer * buffer);
void ALAudioSource_play(ALAudioSource * self);
void ALAudioSource_stop(ALAudioSource * self);
void ALAudioSource_pause(ALAudioSource * self);
// TODO: Loop
// TODO: Streaming

#endif
