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

#include "vorbisaudioio/VorbisAudioIO.h"
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

static size_t memreadFunc(void * outData, size_t size, size_t nmemb, void * inData) {
	struct memreadContext * context = inData;
	
	if (memread(context, size * nmemb, outData)) {
		return size * nmemb;
	}
	if (context->position < context->length) {
		size_t bytesRead;
		
		bytesRead = context->length - context->position;
		memcpy(outData, context->data + context->position, bytesRead);
		context->position = context->length;
		return bytesRead;
	}
	return 0;
}

static int memseekFunc(void * inData, ogg_int64_t offset, int whence) {
	struct memreadContext * context = inData;
	
	switch (whence) {
		case SEEK_SET:
			break;
			
		case SEEK_CUR:
			offset += context->position;
			break;
			
		case SEEK_END:
			offset = context->length;
			break;
			
		default:
			return -1;
	}
	
	if (offset >= 0 && offset <= (ogg_int64_t) context->length) {
		context->position = offset;
		return 0;
	}
	return -1;
}

static long memtellFunc(void * inData) {
	struct memreadContext * context = inData;
	
	return context->position;
}

#define BUFFER_SIZE 16384

#ifdef __BIG_ENDIAN__
#define OV_READ_ENDIANNESS 1
#else
#define OV_READ_ENDIANNESS 0
#endif

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
	callbacks.read_func = memreadFunc;
	callbacks.seek_func = memseekFunc;
	callbacks.close_func = NULL;
	callbacks.tell_func = memtellFunc;
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
