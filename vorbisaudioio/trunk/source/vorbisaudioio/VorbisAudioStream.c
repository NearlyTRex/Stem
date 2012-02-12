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

#include "vorbisaudioio/VorbisAudioStream.h"
#include "vorbisaudioio/VorbisAudioIOPrivate.h"
#include "vorbis/codec.h"

#define SUPERCLASS PCMAudioStream

VorbisAudioStream * VorbisAudioStream_createWithFile(const char * filePath) {
	stemobject_create_implementation(VorbisAudioStream, initWithFile, filePath)
}

VorbisAudioStream * VorbisAudioStream_createWithData(void * data, size_t length, bool takeOwnership) {
	stemobject_create_implementation(VorbisAudioStream, initWithData, data, length, takeOwnership)
}

static void sharedInit(VorbisAudioStream * self) {
	call_super(init, self);
	
	self->dataOwned = false;
	self->currentSection = 0;
	
	self->dispose = VorbisAudioStream_dispose;
	self->read = VorbisAudioStream_read;
	self->seek = VorbisAudioStream_seek;
}

static void sharedGetInfo(VorbisAudioStream * self) {
	vorbis_info * info;
	
	info = ov_info(&self->vorbisFile, -1);
	if (info == NULL) {
		fprintf(stderr, "Error: ov_info returned NULL\n");
		return;
	}
	self->bytesPerSample = 2;
	self->channelCount = info->channels;
	self->sampleRate = info->rate;
	self->sampleCount = ov_pcm_total(&self->vorbisFile, -1);
}

void VorbisAudioStream_initWithFile(VorbisAudioStream * self, const char * filePath) {
	int status;
	
	sharedInit(self);
	status = ov_fopen(filePath, &self->vorbisFile);
	if (status != 0) {
		fprintf(stderr, "Error: ov_fopen returned %d\n", status);
		return;
	}
	sharedGetInfo(self);
}

void VorbisAudioStream_initWithData(VorbisAudioStream * self, void * data, size_t length, bool takeOwnership) {
	ov_callbacks callbacks;
	int status;
	
	sharedInit(self);
	
	self->readContext = memreadContextInit(data, length);
	self->dataOwned = takeOwnership;
	
	callbacks.read_func = VorbisAudioIO_memreadFunc;
	callbacks.seek_func = VorbisAudioIO_memseekFunc;
	callbacks.close_func = NULL;
	callbacks.tell_func = VorbisAudioIO_memtellFunc;
	status = ov_open_callbacks(&self->readContext, &self->vorbisFile, NULL, 0, callbacks);
	if (status != 0) {
		fprintf(stderr, "Error: ov_open_callbacks returned %d\n", status);
		return;
	}
	sharedGetInfo(self);
}

void VorbisAudioStream_dispose(VorbisAudioStream * self) {
	
	ov_clear(&self->vorbisFile);
	if (self->dataOwned) {
		free((void *) self->readContext.data);
	}
	call_super(dispose, self);
}

size_t VorbisAudioStream_read(VorbisAudioStream * self, size_t length, void * buffer, bool loop) {
	long readResult;
	long bytesRead = 0;
	
	while (length > 0) {
		readResult = ov_read(&self->vorbisFile, buffer + bytesRead, length, OV_READ_ENDIANNESS, 2, 1, &self->currentSection);
		if (readResult == 0) {
			if (loop) {
				ov_pcm_seek(&self->vorbisFile, 0);
			} else {
				break;
			}
		} else if (readResult < 0) {
			fprintf(stderr, "Error: ov_read returned %ld\n", readResult);
			break;
		}
		length -= readResult;
		bytesRead += readResult;
	}
	return bytesRead;
}

void VorbisAudioStream_seek(VorbisAudioStream * self, long offset, int whence) {
	switch (whence) {
		case SEEK_CUR:
			offset += ov_pcm_tell(&self->vorbisFile);
			break;
			
		case SEEK_END:
			offset += self->sampleCount;
			break;
	}
	ov_pcm_seek(&self->vorbisFile, offset);
}
