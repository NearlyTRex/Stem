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

#include "vorbisaudioio/VorbisAudioIO.h"
#include "vorbisaudioio/VorbisAudioIOPrivate.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#include "utilities/IOUtilities.h"
#include <string.h>

PCMAudio * VorbisAudioIO_loadOggVorbisFile(const char * filePath) {
	PCMAudio * audio;
	void * fileContents;
	size_t length = 0;
	
	fileContents = readFileSimple(filePath, &length);
	audio = VorbisAudioIO_loadOggVorbisData(fileContents, length);
	free(fileContents);
	
	return audio;
}

#define BUFFER_SIZE 16384

PCMAudio * VorbisAudioIO_loadOggVorbisData(const void * data, size_t length) {
	struct memreadContext readContext;
	ov_callbacks callbacks;
	OggVorbis_File file;
	int status;
	vorbis_info * info;
	unsigned int channelCount;
	unsigned int sampleRate;
	unsigned int sampleCount;
	long readResult;
	struct memwriteContext writeContext;
	char buffer[BUFFER_SIZE];
	int currentSection;
	PCMAudio * audio;
	
	readContext = memreadContextInit(data, length);
	callbacks.read_func = VorbisAudioIO_memreadFunc;
	callbacks.seek_func = VorbisAudioIO_memseekFunc;
	callbacks.close_func = NULL;
	callbacks.tell_func = VorbisAudioIO_memtellFunc;
	status = ov_open_callbacks(&readContext, &file, NULL, 0, callbacks);
	if (status != 0) {
		fprintf(stderr, "Error: ov_open_callbacks returned %d\n", status);
		return NULL;
	}
	
	info = ov_info(&file, -1);
	if (info == NULL) {
		fprintf(stderr, "Error: ov_info returned NULL\n");
		return NULL;
	}
	channelCount = info->channels;
	sampleRate = info->rate;
	
	sampleCount = ov_pcm_total(&file, -1);
	
	writeContext = memwriteContextInit(malloc(1), 0, 1, true);
	for (;;) {
		readResult = ov_read(&file, buffer, BUFFER_SIZE, OV_READ_ENDIANNESS, 2, 1, &currentSection);
		if (readResult == 0) {
			break;
		}
		if (readResult < 0) {
			fprintf(stderr, "Error: ov_read returned %ld\n", readResult);
			break;
		}
		memwrite(&writeContext, readResult, buffer);
	}
	
	ov_clear(&file);
	if (readResult < 0) {
		free(writeContext.data);
		return NULL;
	}
	
	audio = PCMAudio_create(2, channelCount, sampleRate, sampleCount, writeContext.data, true);
	free(writeContext.data);
	return audio;
}

bool VorbisAudioIO_writeOggVorbisFile(PCMAudio * audio, const char * filePath) {
	return false;
}

void * VorbisAudioIO_writeOggVorbisData(PCMAudio * audio, size_t * outLength) {
	return NULL;
}
