/*
  Copyright (c) 2010 Alex Diener
  
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

#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

typedef struct Preferences Preferences;
typedef const char * PreferencesDomain;

#define PREFERENCES_DOMAIN_USER "user"
#define PREFERENCES_DOMAIN_MACHINE "machine"

// EventData is a struct PreferencesEvent *. If the event handler modifies the event's value,
// the modified value will be saved instead of the original value. If the value is of a string
// or data type and the event handler reassigns the string/bytes pointer, the assigned pointer
// must remain valid after the event handler completes, but it is the handler's responsibility
// to ensure that the pointer is subsequently freed.
#define PREFERENCES_EVENT_VALUE_CHANGED "PREFERENCES_EVENT_VALUE_CHANGED"

#include <stdlib.h>
#include <stdbool.h>

enum PreferencesType {
	PREFERENCES_TYPE_INTEGER,
	PREFERENCES_TYPE_FLOAT,
	PREFERENCES_TYPE_BOOLEAN,
	PREFERENCES_TYPE_STRING,
	PREFERENCES_TYPE_DATA
};

union PreferencesValue {
	int integer;
	float number;
	bool boolean;
	char * string;
	struct {
		void * bytes;
		size_t length;
	} data;
};

struct PreferencesEvent {
	const char * name;
	enum PreferencesType type;
	union PreferencesValue value;
};

#include "stemobject/StemObject.h"
#include "utilities/EventDispatcher.h"

struct Preferences_valueRecord {
	char * name;
	enum PreferencesType type;
	union PreferencesValue value;
	union PreferencesValue defaultValue;
};

#define Preferences_structContents \
	StemObject_structContents \
	\
	size_t valueCount; \
	struct Preferences_valueRecord * values; \
	EventDispatcher * eventDispatcher; \
	\
	void (* addInteger)(compat_type(Preferences *) self, const char * name, int defaultValue); \
	void (* addFloat)(compat_type(Preferences *) self, const char * name, float defaultValue); \
	void (* addBoolean)(compat_type(Preferences *) self, const char * name, bool defaultValue); \
	void (* addString)(compat_type(Preferences *) self, const char * name, const char * defaultValue); \
	void (* addData)(compat_type(Preferences *) self, const char * name, void * defaultValue, size_t defaultLength); \
	\
	int (* getInteger)(compat_type(Preferences *) self, const char * name); \
	float (* getFloat)(compat_type(Preferences *) self, const char * name); \
	bool (* getBoolean)(compat_type(Preferences *) self, const char * name); \
	const char * (* getString)(compat_type(Preferences *) self, const char * name); \
	void * (* getData)(compat_type(Preferences *) self, const char * name, size_t * outLength); \
	\
	void (* setInteger)(compat_type(Preferences *) self, const char * name, int value); \
	void (* setFloat)(compat_type(Preferences *) self, const char * name, float value); \
	void (* setBoolean)(compat_type(Preferences *) self, const char * name, bool value); \
	void (* setString)(compat_type(Preferences *) self, const char * name, const char * value); \
	void (* setData)(compat_type(Preferences *) self, const char * name, void * value, size_t length); \
	\
	void (* load)(compat_type(Preferences *) self); \
	void (* save)(compat_type(Preferences *) self); \
	void (* loadDefaultValues)(compat_type(Preferences *) self); \
	void (* loadDefaultValue)(compat_type(Preferences *) self, const char * name);

struct Preferences {
	Preferences_structContents
};

Preferences * Preferences_create(const char * identifier, PreferencesDomain domain);
void Preferences_init(compat_type(Preferences *) selfPtr, const char * identifier, PreferencesDomain domain);
void Preferences_dispose(compat_type(Preferences *) selfPtr);

void Preferences_addInteger(compat_type(Preferences *) selfPtr, const char * name, int defaultValue);
void Preferences_addFloat(compat_type(Preferences *) selfPtr, const char * name, float defaultValue);
void Preferences_addBoolean(compat_type(Preferences *) selfPtr, const char * name, bool defaultValue);
void Preferences_addString(compat_type(Preferences *) selfPtr, const char * name, const char * defaultValue);
void Preferences_addData(compat_type(Preferences *) selfPtr, const char * name, void * defaultValue, size_t defaultLength);

int Preferences_getInteger(compat_type(Preferences *) selfPtr, const char * name);
float Preferences_getFloat(compat_type(Preferences *) selfPtr, const char * name);
bool Preferences_getBoolean(compat_type(Preferences *) selfPtr, const char * name);
const char * Preferences_getString(compat_type(Preferences *) selfPtr, const char * name);
void * Preferences_getData(compat_type(Preferences *) selfPtr, const char * name, size_t * outLength);

void Preferences_setInteger(compat_type(Preferences *) selfPtr, const char * name, int value);
void Preferences_setFloat(compat_type(Preferences *) selfPtr, const char * name, float value);
void Preferences_setBoolean(compat_type(Preferences *) selfPtr, const char * name, bool value);
void Preferences_setString(compat_type(Preferences *) selfPtr, const char * name, const char * value);
void Preferences_setData(compat_type(Preferences *) selfPtr, const char * name, void * value, size_t length);

void Preferences_load(compat_type(Preferences *) selfPtr);
void Preferences_save(compat_type(Preferences *) selfPtr);
void Preferences_loadDefaultValues(compat_type(Preferences *) selfPtr);
void Preferences_loadDefaultValue(compat_type(Preferences *) selfPtr, const char * name);

#endif
