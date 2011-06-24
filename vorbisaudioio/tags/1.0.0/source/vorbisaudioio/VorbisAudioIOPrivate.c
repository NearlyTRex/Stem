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

#include "vorbisaudioio/VorbisAudioIOPrivate.h"
#include "utilities/IOUtilities.h"
#include <string.h>

size_t VorbisAudioIO_memreadFunc(void * outData, size_t size, size_t nmemb, void * inData) {
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

int VorbisAudioIO_memseekFunc(void * inData, ogg_int64_t offset, int whence) {
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

long VorbisAudioIO_memtellFunc(void * inData) {
	struct memreadContext * context = inData;
	
	return context->position;
}
