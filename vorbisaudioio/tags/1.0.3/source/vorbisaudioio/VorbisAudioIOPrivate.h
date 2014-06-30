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

#ifndef __VorbisAudioIOPrivate_H__
#define __VorbisAudioIOPrivate_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#ifdef __BIG_ENDIAN__
#define OV_READ_ENDIANNESS 1
#else
#define OV_READ_ENDIANNESS 0
#endif

size_t VorbisAudioIO_memreadFunc(void * outData, size_t size, size_t nmemb, void * inData);
int VorbisAudioIO_memseekFunc(void * inData, ogg_int64_t offset, int whence);
long VorbisAudioIO_memtellFunc(void * inData);

#ifdef __cplusplus
}
#endif
#endif
