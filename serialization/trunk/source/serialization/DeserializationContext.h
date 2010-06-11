/*
  Copyright (c) 2010 Alex Diener
  
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

#ifndef __DESERIALIZATION_CONTEXT_H__
#define __DESERIALIZATION_CONTEXT_H__

#include "serialization/SerializationShared.h"
#include "stemobject/StemObject.h"
#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>

typedef struct DeserializationContext DeserializationContext;

#define DeserializationContext_structContents \
	StemObject_structContents \
	\
	jmp_buf * jmpBuf; \
	int status; \
	\
	/* Implementations should return the number of elements in the container */ \
	size_t (* beginStructure)(void * self, const char * key); \
	size_t (* beginDictionary)(void * self, const char * key); \
	size_t (* beginArray)(void * self, const char * key); \
	\
	void (* endStructure)(void * self); \
	void (* endDictionary)(void * self); \
	void (* endArray)(void * self); \
	\
	int8_t       (* readInt8)(void * self, const char * key); \
	uint8_t      (* readUInt8)(void * self, const char * key); \
	int16_t      (* readInt16)(void * self, const char * key); \
	uint16_t     (* readUInt16)(void * self, const char * key); \
	int32_t      (* readInt32)(void * self, const char * key); \
	uint32_t     (* readUInt32)(void * self, const char * key); \
	int64_t      (* readInt64)(void * self, const char * key); \
	uint64_t     (* readUInt64)(void * self, const char * key); \
	float        (* readFloat)(void * self, const char * key); \
	double       (* readDouble)(void * self, const char * key); \
	/* Returned string not owned by caller; do not free */ \
	const char * (* readString)(void * self, const char * key); \
	bool         (* readBoolean)(void * self, const char * key); \
	\
	/* Additional args: Pairs of strings naming constants, and the values described by them, terminated by */ \
	/* NULL. You can use the enumKV macro from serialization/SerializationShared.h for convenience. */ \
	int          (* readEnumeration)(void * self, const char * key, ...); \
	\
	/* Additional args: Strings naming each bit from least significant to most significant, up to the maximum */ \
	/* number of bits in the field, or terminated by NULL if there are unused high bits. */ \
	uint8_t      (* readBitfield8)(void * self, const char * key, ...); \
	uint16_t     (* readBitfield16)(void * self, const char * key, ...); \
	uint32_t     (* readBitfield32)(void * self, const char * key, ...); \
	uint64_t     (* readBitfield64)(void * self, const char * key, ...); \
	\
	/* Returned string not owned by caller; do not free */ \
	/* Valid only when reading an ordered dictionary */ \
	const char * (* readNextDictionaryKey)(void * self); \
	\
	/* Valid only when reading an unordered dictionary */ \
	bool         (* hasDictionaryKey)(void * self, const char * key);

struct DeserializationContext {
	DeserializationContext_structContents
};

void DeserializationContext_init(DeserializationContext * self);
void DeserializationContext_dispose(void * self);

#endif
