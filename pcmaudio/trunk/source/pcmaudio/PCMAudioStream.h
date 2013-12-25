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

#ifndef __PCM_AUDIO_STREAM_H__
#define __PCM_AUDIO_STREAM_H__

typedef struct PCMAudioStream PCMAudioStream;

#include "stemobject/StemObject.h"
#include <stdbool.h>
#include <stdlib.h>

#define PCMAudioStream_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	unsigned int bytesPerSample; \
	unsigned int channelCount; \
	unsigned int sampleRate; \
	size_t sampleCount; \
	\
	/* length is in bytes; if loop is true, audio will seamlessly wrap to the beginning if the end is reached during that read call */ \
	size_t (* read)(self_type * self, size_t length, void * buffer, bool loop); \
	/* offset is in sample frames (bytesPerSample * channelCount bytes); whence is SEEK_SET, SEEK_CUR, or SEEK_END */ \
	void (* seek)(self_type * self, long offset, int whence);

stemobject_struct_definition(PCMAudioStream)

bool PCMAudioStream_init(PCMAudioStream * self);
void PCMAudioStream_dispose(PCMAudioStream * self);

#endif
