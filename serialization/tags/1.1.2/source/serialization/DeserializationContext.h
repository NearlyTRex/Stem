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

#define DeserializationContext_structContents \
	StemObject_structContents \
	\
	jmp_buf * jmpBuf; \
	int status; \
	\
	/* Implementations should return the number of elements in the container (except beginStructure) */ \
	void (* beginStructure)(compat_type(DeserializationContext *) self, const char * key); \
	size_t (* beginDictionary)(compat_type(DeserializationContext *) self, const char * key); \
	size_t (* beginArray)(compat_type(DeserializationContext *) self, const char * key); \
	\
	void (* endStructure)(compat_type(DeserializationContext *) self); \
	void (* endDictionary)(compat_type(DeserializationContext *) self); \
	void (* endArray)(compat_type(DeserializationContext *) self); \
	\
	int8_t       (* readInt8)(compat_type(DeserializationContext *) self, const char * key); \
	uint8_t      (* readUInt8)(compat_type(DeserializationContext *) self, const char * key); \
	int16_t      (* readInt16)(compat_type(DeserializationContext *) self, const char * key); \
	uint16_t     (* readUInt16)(compat_type(DeserializationContext *) self, const char * key); \
	int32_t      (* readInt32)(compat_type(DeserializationContext *) self, const char * key); \
	uint32_t     (* readUInt32)(compat_type(DeserializationContext *) self, const char * key); \
	int64_t      (* readInt64)(compat_type(DeserializationContext *) self, const char * key); \
	uint64_t     (* readUInt64)(compat_type(DeserializationContext *) self, const char * key); \
	float        (* readFloat)(compat_type(DeserializationContext *) self, const char * key); \
	double       (* readDouble)(compat_type(DeserializationContext *) self, const char * key); \
	/* Returned string not owned by caller; do not free */ \
	const char * (* readString)(compat_type(DeserializationContext *) self, const char * key); \
	bool         (* readBoolean)(compat_type(DeserializationContext *) self, const char * key); \
	\
	/* Additional args: Pairs of strings naming constants, and the values described by them, terminated by */ \
	/* NULL. You can use the enumKV macro from serialization/SerializationShared.h for convenience. */ \
	int          (* readEnumeration)(compat_type(DeserializationContext *) self, const char * key, ...); \
	\
	/* Additional args: Strings naming each bit from least significant to most significant, up to the maximum */ \
	/* number of bits in the field, or terminated by NULL if there are unused high bits. */ \
	uint8_t      (* readBitfield8)(compat_type(DeserializationContext *) self, const char * key, ...); \
	uint16_t     (* readBitfield16)(compat_type(DeserializationContext *) self, const char * key, ...); \
	uint32_t     (* readBitfield32)(compat_type(DeserializationContext *) self, const char * key, ...); \
	uint64_t     (* readBitfield64)(compat_type(DeserializationContext *) self, const char * key, ...); \
	\
	/* Returned string not owned by caller; do not free */ \
	/* Valid only when reading an ordered dictionary */ \
	const char * (* readNextDictionaryKey)(compat_type(DeserializationContext *) self); \
	\
	/* Valid only when reading an unordered dictionary */ \
	bool         (* hasDictionaryKey)(compat_type(DeserializationContext *) self, const char * key);

struct DeserializationContext {
	DeserializationContext_structContents
};

void DeserializationContext_init(compat_type(DeserializationContext *) selfPtr);
void DeserializationContext_dispose(compat_type(DeserializationContext *) selfPtr);

#endif
