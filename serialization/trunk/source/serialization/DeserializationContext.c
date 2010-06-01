#include "serialization/DeserializationContext.h"
#include <stdlib.h>

void DeserializationContext_init(DeserializationContext * self) {
	StemObject_init((StemObject *) self);
	
	self->jmpBuf = NULL;
	self->status = SERIALIZATION_ERROR_OK;
	self->dispose = DeserializationContext_dispose;
	self->beginStructure = NULL;
	self->beginDictionary = NULL;
	self->beginArray = NULL;
	self->endStructure = NULL;
	self->endDictionary = NULL;
	self->endArray = NULL;
	self->readInt8 = NULL;
	self->readUInt8 = NULL;
	self->readInt16 = NULL;
	self->readUInt16 = NULL;
	self->readInt32 = NULL;
	self->readUInt32 = NULL;
	self->readInt64 = NULL;
	self->readUInt64 = NULL;
	self->readFloat = NULL;
	self->readDouble = NULL;
	self->readNextDictionaryKey = NULL;
	self->readString = NULL;
	self->readBoolean = NULL;
	self->readEnumeration = NULL;
	self->readBitfield8 = NULL;
	self->readBitfield16 = NULL;
	self->readBitfield32 = NULL;
	self->readBitfield64 = NULL;
}

void DeserializationContext_dispose(void * selfPtr) {
	StemObject_dispose(selfPtr);
}
