/*
  Copyright (c) 2014 Alex Diener
  
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
  
  Alex Diener alex@ludobloom.com
*/

#ifndef __SerializationContext_H__
#define __SerializationContext_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "gamemath/FixedPoint.h"
#include "serialization/SerializationShared.h"
#include "stemobject/StemObject.h"
#include <stdlib.h>
#include <stdint.h>
#include <setjmp.h>

typedef struct SerializationContext SerializationContext;

#define SerializationContext_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	jmp_buf * jmpBuf; \
	int status; \
	\
	const char * (* errorString)(int status); \
	\
	void (* beginStructure)(self_type * self, const char * key); \
	void (* beginDictionary)(self_type * self, const char * key); \
	void (* beginArray)(self_type * self, const char * key); \
	\
	void (* endStructure)(self_type * self); \
	void (* endDictionary)(self_type * self); \
	void (* endArray)(self_type * self); \
	\
	void (* writeBoolean)(self_type * self, const char * key, bool value); \
	void (* writeInt8)(self_type * self, const char * key, int8_t value); \
	void (* writeUInt8)(self_type * self, const char * key, uint8_t value); \
	void (* writeInt16)(self_type * self, const char * key, int16_t value); \
	void (* writeUInt16)(self_type * self, const char * key, uint16_t value); \
	void (* writeInt32)(self_type * self, const char * key, int32_t value); \
	void (* writeUInt32)(self_type * self, const char * key, uint32_t value); \
	void (* writeInt64)(self_type * self, const char * key, int64_t value); \
	void (* writeUInt64)(self_type * self, const char * key, uint64_t value); \
	void (* writeFloat)(self_type * self, const char * key, float value); \
	void (* writeDouble)(self_type * self, const char * key, double value); \
	void (* writeFixed16_16)(self_type * self, const char * key, fixed16_16 value); \
	\
	/* Additional args: Pairs of strings naming constants, and the values described by them, terminated by */ \
	/* NULL. You can use the enumKV macro from serialization/SerializationShared.h for convenience. */ \
	void (* struct_fptr_sentinel writeEnumeration)(self_type * self, const char * key, int value, ...); \
	\
	/* Additional args: Strings naming each bit from least significant to most significant, up to the maximum */ \
	/* number of bits in the field, or terminated by NULL if there are unused bits at the top. */ \
	void (* struct_fptr_sentinel writeBitfield8)(self_type * self, const char * key, uint8_t value, ...); \
	void (* struct_fptr_sentinel writeBitfield16)(self_type * self, const char * key, uint16_t value, ...); \
	void (* struct_fptr_sentinel writeBitfield32)(self_type * self, const char * key, uint32_t value, ...); \
	void (* struct_fptr_sentinel writeBitfield64)(self_type * self, const char * key, uint64_t value, ...); \
	\
	void (* writeString)(self_type * self, const char * key, const char * value); \
	void (* writeBlob)(self_type * self, const char * key, const void * bytes, size_t length);

stemobject_struct_definition(SerializationContext)

bool SerializationContext_init(SerializationContext * self);
void SerializationContext_dispose(SerializationContext * self);

#ifdef __cplusplus
}
#endif
#endif
