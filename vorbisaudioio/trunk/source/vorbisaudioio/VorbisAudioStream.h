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

#ifndef __VORBIS_AUDIO_STREAM_H__
#define __VORBIS_AUDIO_STREAM_H__

typedef struct VorbisAudioStream VorbisAudioStream;

#include "pcmaudio/PCMAudioStream.h"
#include <stdio.h>
#include <stdlib.h>
#include "vorbis/vorbisfile.h"
#include "utilities/IOUtilities.h"

#define VorbisAudioStream_structContents(self_type) \
	PCMAudioStream_structContents(self_type) \
	\
	struct memreadContext readContext; \
	bool dataOwned; \
	OggVorbis_File vorbisFile; \
	int currentSection;

stemobject_struct_definition(VorbisAudioStream)

VorbisAudioStream * VorbisAudioStream_createWithFile(const char * filePath);
VorbisAudioStream * VorbisAudioStream_createWithData(void * data, size_t length, bool takeOwnership);
void VorbisAudioStream_initWithFile(VorbisAudioStream * self, const char * filePath);
void VorbisAudioStream_initWithData(VorbisAudioStream * self, void * data, size_t length, bool takeOwnership);

void VorbisAudioStream_dispose(VorbisAudioStream * self);
size_t VorbisAudioStream_read(VorbisAudioStream * self, size_t length, void * buffer, bool loop);
void VorbisAudioStream_seek(VorbisAudioStream * self, long offset, int whence);

#endif
