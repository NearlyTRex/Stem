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

#ifndef __Preferences_H__
#define __Preferences_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Preferences Preferences;

// EventData is a struct PreferencesEvent *. If the event handler modifies the event's value,
// the modified value will be saved instead of the original value.
#define PREFERENCES_EVENT_VALUE_CHANGED "PREFERENCES_EVENT_VALUE_CHANGED"

#include <stdlib.h>
#include <stdbool.h>
#include "dynamictypes/DataValue.h"
#include "dynamictypes/HashTable.h"
#include "stemobject/StemObject.h"
#include "utilities/EventDispatcher.h"

struct PreferencesEvent {
	const char * key;
	DataValue value;
	DataValue previousValue;
};

#define Preferences_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	HashTable * private_ivar(hashTable); \
	EventDispatcher * eventDispatcher;

stemobject_struct_definition(Preferences)

void Preferences_getFilePath(const char * fileName, char * outPath, size_t maxLength);

Preferences * Preferences_create(void);
bool Preferences_init(Preferences * self);
void Preferences_dispose(Preferences * self);

void Preferences_set(Preferences * self, const char * key, DataValue value);
// Return value not owned by caller; don't modify or dispose
DataValue * Preferences_get(Preferences * self, const char * key);

void Preferences_import(Preferences * self, HashTable * hashTable);
// Return value owned by caller; call hashDispose when done
HashTable * Preferences_export(Preferences * self);

#ifdef __cplusplus
}
#endif
#endif
