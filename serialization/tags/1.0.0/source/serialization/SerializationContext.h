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
	void (* beginStructure)(void * self, char * key); \
	void (* beginDictionary)(void * self, char * key); \
	void (* beginArray)(void * self, char * key); \
	\
	void (* endStructure)(void * self); \
	void (* endDictionary)(void * self); \
	void (* endArray)(void * self); \
	\
	void (* writeInt8)(void * self, char * key, int8_t value); \
	void (* writeUInt8)(void * self, char * key, uint8_t value); \
	void (* writeInt16)(void * self, char * key, int16_t value); \
	void (* writeUInt16)(void * self, char * key, uint16_t value); \
	void (* writeInt32)(void * self, char * key, int32_t value); \
	void (* writeUInt32)(void * self, char * key, uint32_t value); \
	void (* writeInt64)(void * self, char * key, int64_t value); \
	void (* writeUInt64)(void * self, char * key, uint64_t value); \
	void (* writeFloat)(void * self, char * key, float value); \
	void (* writeDouble)(void * self, char * key, double value); \
	void (* writeString)(void * self, char * key, char * value); \
	void (* writeBoolean)(void * self, char * key, bool value); \
	\
	/* Additional args: Pairs of strings naming constants, and the values described by them, terminated by */ \
	/* NULL. You can use the enumKV macro from serialization/SerializationShared.h for convenience. */ \
	void (* writeEnumeration)(void * self, char * key, int value, ...); \
	\
	/* Additional args: Strings naming each bit from least significant to most significant, up to the maximum */ \
	/* number of bits in the field, or terminated by NULL if there are unused bits at the top. */ \
	void (* writeBitfield8)(void * self, char * key, uint8_t value, ...); \
	void (* writeBitfield16)(void * self, char * key, uint16_t value, ...); \
	void (* writeBitfield32)(void * self, char * key, uint32_t value, ...); \
	void (* writeBitfield64)(void * self, char * key, uint64_t value, ...);

struct SerializationContext {
	SerializationContext_structContents
};

void SerializationContext_init(SerializationContext * self);
void SerializationContext_dispose(void * selfPtr);

#endif
