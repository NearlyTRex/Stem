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

#ifndef __AL_AUDIO_SOURCE_H__
#define __AL_AUDIO_SOURCE_H__

typedef struct ALAudioSource ALAudioSource;

#include "stemobject/StemObject.h"
#include "alaudio/ALAudioBuffer.h"

#define ALAudioSource_structContents \
	StemObject_structContents \
	\
	ALuint source; \
	void (* load)(compat_type(ALAudioSource *) self, ALAudioBuffer * buffer); \
	void (* play)(compat_type(ALAudioSource *) self); \
	void (* stop)(compat_type(ALAudioSource *) self); \
	void (* pause)(compat_type(ALAudioSource *) self);

struct ALAudioSource {
	ALAudioSource_structContents
};

ALAudioSource * ALAudioSource_create();
void ALAudioSource_init(compat_type(ALAudioSource *) selfPtr);
void ALAudioSource_dispose(compat_type(ALAudioSource *) selfPtr);

void ALAudioSource_load(compat_type(ALAudioSource *) selfPtr, ALAudioBuffer * buffer);
void ALAudioSource_play(compat_type(ALAudioSource *) selfPtr);
void ALAudioSource_stop(compat_type(ALAudioSource *) selfPtr);
void ALAudioSource_pause(compat_type(ALAudioSource *) selfPtr);
// TODO: Loop
// TODO: Streaming

#endif
