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

#include "preferences/Preferences.h"
#include "preferences/Preferences_private.h"
#include <string.h>

#define SUPERCLASS StemObject

void Preferences_getFilePath(const char * fileName, char * outPath, size_t maxLength) {
	Preferences_getFilePathPrivate(fileName, outPath, maxLength);
}

Preferences * Preferences_create(void) {
	stemobject_create_implementation(Preferences, init)
}

bool Preferences_init(Preferences * self) {
	call_super(init, self);
	
	self->private_ivar(hashTable) = hashCreate();
	self->eventDispatcher = EventDispatcher_create();
	
	self->dispose = Preferences_dispose;
	return true;
}

void Preferences_dispose(Preferences * self) {
	hashDispose(self->private_ivar(hashTable));
	call_super(dispose, self);
}

void Preferences_set(Preferences * self, const char * key, DataValue value) {
	struct PreferencesEvent event;
	DataValue * previousValue;
	
	event.key = key;
	event.value = value;
	previousValue = hashGet(self->private_ivar(hashTable), key);
	if (previousValue == NULL) {
		memset(&event.previousValue, 0, sizeof(event.previousValue));
		event.previousValue.type = value.type;
	} else {
		event.previousValue = *previousValue;
	}
	EventDispatcher_dispatchEvent(self->eventDispatcher, ATOM(PREFERENCES_EVENT_VALUE_CHANGED), &event);
	
	hashSet(self->private_ivar(hashTable), key, valueCopy(&event.value));
}

DataValue * Preferences_get(Preferences * self, const char * key) {
	return hashGet(self->private_ivar(hashTable), key);
}

void Preferences_import(Preferences * self, HashTable * hashTable) {
	const char ** keys;
	size_t keyCount = 0, keyIndex;
	
	keys = hashGetKeys(hashTable, &keyCount);
	for (keyIndex = 0; keyIndex < keyCount; keyIndex++) {
		Preferences_set(self, keys[keyIndex], *hashGet(hashTable, keys[keyIndex]));
	}
	free(keys);
}

HashTable * Preferences_export(Preferences * self) {
	return hashCopy(self->private_ivar(hashTable));
}
