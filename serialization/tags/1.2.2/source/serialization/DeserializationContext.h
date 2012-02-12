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

#ifndef __DESERIALIZATION_CONTEXT_H__
#define __DESERIALIZATION_CONTEXT_H__

typedef struct DeserializationContext DeserializationContext;

#include "serialization/SerializationShared.h"
#include "stemobject/StemObject.h"
#include <stdint.h>
#include <stdlib.h>
#include <setjmp.h>

#define stemobject_deserialize_implementation(class_name, deserialization_context, load_suffix) \
	DeserializationContext * context = deserialization_context; \
	class_name * self; \
	\
	self = malloc(sizeof(class_name)); \
	if (!class_name##_##load_suffix(self, context)) { \
		free(self); \
		return NULL; \
	} \
	self->protected_ivar(allocated) = true; \
	\
	return self;

#define DeserializationContext_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	jmp_buf * jmpBuf; \
	int status; \
	\
	/* Implementations should return the number of elements in the container (except beginStructure) */ \
	void (* beginStructure)(self_type * self, const char * key); \
	size_t (* beginDictionary)(self_type * self, const char * key); \
	size_t (* beginArray)(self_type * self, const char * key); \
	\
	void (* endStructure)(self_type * self); \
	void (* endDictionary)(self_type * self); \
	void (* endArray)(self_type * self); \
	\
	int8_t       (* readInt8)(self_type * self, const char * key); \
	uint8_t      (* readUInt8)(self_type * self, const char * key); \
	int16_t      (* readInt16)(self_type * self, const char * key); \
	uint16_t     (* readUInt16)(self_type * self, const char * key); \
	int32_t      (* readInt32)(self_type * self, const char * key); \
	uint32_t     (* readUInt32)(self_type * self, const char * key); \
	int64_t      (* readInt64)(self_type * self, const char * key); \
	uint64_t     (* readUInt64)(self_type * self, const char * key); \
	float        (* readFloat)(self_type * self, const char * key); \
	double       (* readDouble)(self_type * self, const char * key); \
	/* Returned string not owned by caller; do not free */ \
	const char * (* readString)(self_type * self, const char * key); \
	bool         (* readBoolean)(self_type * self, const char * key); \
	const void * (* readBytes)(self_type * self, const char * key, size_t * outLength); \
	\
	/* Additional args: Pairs of strings naming constants, and the values described by them, terminated by */ \
	/* NULL. You can use the enumKV macro from serialization/SerializationShared.h for convenience. */ \
	int          (* struct_fptr_sentinel readEnumeration)(self_type * self, const char * key, ...); \
	\
	/* Additional args: Strings naming each bit from least significant to most significant, up to the maximum */ \
	/* number of bits in the field, or terminated by NULL if there are unused high bits. */ \
	uint8_t      (* struct_fptr_sentinel readBitfield8)(self_type * self, const char * key, ...); \
	uint16_t     (* struct_fptr_sentinel readBitfield16)(self_type * self, const char * key, ...); \
	uint32_t     (* struct_fptr_sentinel readBitfield32)(self_type * self, const char * key, ...); \
	uint64_t     (* struct_fptr_sentinel readBitfield64)(self_type * self, const char * key, ...); \
	\
	/* Returned string not owned by caller; do not free */ \
	/* Valid only when reading an ordered dictionary */ \
	const char * (* readNextDictionaryKey)(self_type * self); \
	\
	/* Valid only when reading an unordered dictionary */ \
	bool         (* hasDictionaryKey)(self_type * self, const char * key);

stemobject_struct_definition(DeserializationContext)

void DeserializationContext_init(compat_type(DeserializationContext *) selfPtr);
void DeserializationContext_dispose(DeserializationContext * self);

#endif
