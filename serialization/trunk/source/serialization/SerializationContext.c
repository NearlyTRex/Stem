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

#include "serialization/SerializationContext.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

void SerializationContext_init(SerializationContext * self) {
	call_super(init, self);
	
	self->jmpBuf = NULL;
	self->status = SERIALIZATION_ERROR_OK;
	self->dispose = SerializationContext_dispose;
	self->errorString = Serialization_errorString;
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
	self->writeString = NULL;
	self->writeBoolean = NULL;
	self->writeBytes = NULL;
	self->writeEnumeration = NULL;
	self->writeBitfield8 = NULL;
	self->writeBitfield16 = NULL;
	self->writeBitfield32 = NULL;
	self->writeBitfield64 = NULL;
}

void SerializationContext_dispose(SerializationContext * self) {
	call_super(dispose, self);
}

const char * SerializationContext_errorString(int status) {
	return Serialization_errorString(status);
}
