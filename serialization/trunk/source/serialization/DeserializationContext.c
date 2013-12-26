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

#include "serialization/DeserializationContext.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

bool DeserializationContext_init(DeserializationContext * self) {
	call_super(init, self);
	
	self->jmpBuf = NULL;
	self->status = SERIALIZATION_ERROR_OK;
	self->dispose = DeserializationContext_dispose;
	self->errorString = Serialization_errorString;
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
	self->readString = NULL;
	self->readBoolean = NULL;
	self->readBytes = NULL;
	self->readEnumeration = NULL;
	self->readBitfield8 = NULL;
	self->readBitfield16 = NULL;
	self->readBitfield32 = NULL;
	self->readBitfield64 = NULL;
	self->readNextDictionaryKey = NULL;
	self->hasDictionaryKey = NULL;
	return true;
}

void DeserializationContext_dispose(DeserializationContext * self) {
	call_super(dispose, self);
}
