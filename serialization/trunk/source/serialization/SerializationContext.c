#include "serialization/SerializationContext.h"
#include <stdlib.h>

void SerializationContext_init(SerializationContext * self) {
	self->jmpBuf = NULL;
	self->status = 0;
	self->beginStructure = NULL;
	self->beginDictionary = NULL;
	self->beginArray = NULL;
	self->endStructure = NULL;
	self->endDictionary = NULL;
	self->endArray = NULL;
	self->writeInt8 = NULL;
	self->writeUInt8 = NULL;
	self->writeInt16 = NULL;
	self->writeUInt16 = NULL;
	self->writeInt32 = NULL;
	self->writeUInt32 = NULL;
	self->writeInt64 = NULL;
	self->writeUInt64 = NULL;
	self->writeFloat = NULL;
	self->writeDouble = NULL;
	self->writeEnumeration = NULL;
	self->writeString = NULL;
	self->writeBitfield8 = NULL;
	self->writeBitfield16 = NULL;
	self->writeBitfield32 = NULL;
	self->writeBitfield64 = NULL;
}

void SerializationContext_dispose(void * selfPtr) {
	StemObject_dispose(selfPtr);
}
