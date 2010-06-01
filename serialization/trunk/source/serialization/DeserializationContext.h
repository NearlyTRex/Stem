#ifndef __DESERIALIZATION_CONTEXT_H__
#define __DESERIALIZATION_CONTEXT_H__

#include "serialization/SerializationShared.h"
#include "stemobject/StemObject.h"
#include <stdint.h>
#include <setjmp.h>

typedef struct DeserializationContext DeserializationContext;

#define DeserializationContext_structContents \
	StemObject_structContents \
	\
	jmp_buf * jmpBuf; \
	int status; \
	\
	/* Implementations should return the number of elements in the container */ \
	unsigned int (* beginStructure)(void * self, char * key); \
	unsigned int (* beginDictionary)(void * self, char * key); \
	unsigned int (* beginArray)(void * self, char * key); \
	\
	void (* endStructure)(void * self); \
	void (* endDictionary)(void * self); \
	void (* endArray)(void * self); \
	\
	int8_t   (* readInt8)(void * self, char * key); \
	uint8_t  (* readUInt8)(void * self, char * key); \
	int16_t  (* readInt16)(void * self, char * key); \
	uint16_t (* readUInt16)(void * self, char * key); \
	int32_t  (* readInt32)(void * self, char * key); \
	uint32_t (* readUInt32)(void * self, char * key); \
	int64_t  (* readInt64)(void * self, char * key); \
	uint64_t (* readUInt64)(void * self, char * key); \
	float    (* readFloat)(void * self, char * key); \
	double   (* readDouble)(void * self, char * key); \
	char *   (* readNextDictionaryKey)(void * self); \
	char *   (* readString)(void * self, char * key); \
	bool     (* readBoolean)(void * self, char * key); \
	\
	/* Additional args: Pairs of strings naming constants, and the values described by them, terminated by */ \
	/* NULL. You can use the enumKV macro from serialization/SerializationShared.h for convenience. */ \
	int      (* readEnumeration)(void * self, char * key, ...); \
	\
	/* Additional args: Strings naming each bit from least significant to most significant, up to the maximum */ \
	/* number of bits in the field, or terminated by NULL if there are unused bits at the top. */ \
	uint8_t  (* readBitfield8)(void * self, char * key, ...); \
	uint16_t (* readBitfield16)(void * self, char * key, ...); \
	uint32_t (* readBitfield32)(void * self, char * key, ...); \
	uint64_t (* readBitfield64)(void * self, char * key, ...);

struct DeserializationContext {
	DeserializationContext_structContents
};

void DeserializationContext_init(DeserializationContext * self);
void DeserializationContext_dispose(void * self);

#endif
