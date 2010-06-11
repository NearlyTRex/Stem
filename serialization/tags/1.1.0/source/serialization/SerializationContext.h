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

#ifndef __SERIALIZATION_H__
#define __SERIALIZATION_H__

#include "serialization/SerializationShared.h"
#include "stemobject/StemObject.h"
#include <stdint.h>
#include <setjmp.h>

typedef struct SerializationContext SerializationContext;

#define SerializationContext_structContents \
	StemObject_structContents \
	\
	jmp_buf * jmpBuf; \
	int status; \
	\
	void (* beginStructure)(void * self, const char * key); \
	void (* beginDictionary)(void * self, const char * key); \
	void (* beginArray)(void * self, const char * key); \
	\
	void (* endStructure)(void * self); \
	void (* endDictionary)(void * self); \
	void (* endArray)(void * self); \
	\
	void (* writeInt8)(void * self, const char * key, int8_t value); \
	void (* writeUInt8)(void * self, const char * key, uint8_t value); \
	void (* writeInt16)(void * self, const char * key, int16_t value); \
	void (* writeUInt16)(void * self, const char * key, uint16_t value); \
	void (* writeInt32)(void * self, const char * key, int32_t value); \
	void (* writeUInt32)(void * self, const char * key, uint32_t value); \
	void (* writeInt64)(void * self, const char * key, int64_t value); \
	void (* writeUInt64)(void * self, const char * key, uint64_t value); \
	void (* writeFloat)(void * self, const char * key, float value); \
	void (* writeDouble)(void * self, const char * key, double value); \
	void (* writeString)(void * self, const char * key, const char * value); \
	void (* writeBoolean)(void * self, const char * key, bool value); \
	\
	/* Additional args: Pairs of strings naming constants, and the values described by them, terminated by */ \
	/* NULL. You can use the enumKV macro from serialization/SerializationShared.h for convenience. */ \
	void (* writeEnumeration)(void * self, const char * key, int value, ...); \
	\
	/* Additional args: Strings naming each bit from least significant to most significant, up to the maximum */ \
	/* number of bits in the field, or terminated by NULL if there are unused bits at the top. */ \
	void (* writeBitfield8)(void * self, const char * key, uint8_t value, ...); \
	void (* writeBitfield16)(void * self, const char * key, uint16_t value, ...); \
	void (* writeBitfield32)(void * self, const char * key, uint32_t value, ...); \
	void (* writeBitfield64)(void * self, const char * key, uint64_t value, ...);

struct SerializationContext {
	SerializationContext_structContents
};

void SerializationContext_init(SerializationContext * self);
void SerializationContext_dispose(void * selfPtr);

#endif
