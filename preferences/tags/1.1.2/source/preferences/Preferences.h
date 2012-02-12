/*
  Copyright (c) 2012 Alex Diener
  
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

union PreferencesValueConst {
	int integer;
	float number;
	bool boolean;
	const char * string;
	struct {
		const void * bytes;
		size_t length;
	} data;
};

struct PreferencesEvent {
	const char * name;
	enum PreferencesType type;
	union PreferencesValueConst value;
	union PreferencesValueConst previousValue;
};

#include "stemobject/StemObject.h"
#include "utilities/EventDispatcher.h"

struct Preferences_valueRecord {
	char * name;
	enum PreferencesType type;
	union PreferencesValue value;
	union PreferencesValue defaultValue;
};

#define Preferences_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	const char * identifier; \
	size_t valueCount; \
	struct Preferences_valueRecord * values; \
	EventDispatcher * eventDispatcher; \
	\
	void (* addInteger)(self_type * self, const char * name, int defaultValue); \
	void (* addFloat)(self_type * self, const char * name, float defaultValue); \
	void (* addBoolean)(self_type * self, const char * name, bool defaultValue); \
	void (* addString)(self_type * self, const char * name, const char * defaultValue); \
	void (* addData)(self_type * self, const char * name, const void * defaultValue, size_t defaultLength); \
	\
	int (* getInteger)(self_type * self, const char * name); \
	float (* getFloat)(self_type * self, const char * name); \
	bool (* getBoolean)(self_type * self, const char * name); \
	const char * (* getString)(self_type * self, const char * name); \
	void * (* getData)(self_type * self, const char * name, size_t * outLength); \
	\
	void (* setInteger)(self_type * self, const char * name, int value); \
	void (* setFloat)(self_type * self, const char * name, float value); \
	void (* setBoolean)(self_type * self, const char * name, bool value); \
	void (* setString)(self_type * self, const char * name, const char * value); \
	void (* setData)(self_type * self, const char * name, const void * value, size_t length); \
	\
	void (* load)(self_type * self); \
	void (* save)(self_type * self); \
	void (* loadDefaultValues)(self_type * self); \
	void (* loadDefaultValue)(self_type * self, const char * name);

stemobject_struct_definition(Preferences)

void Preferences_getFilePath(const char * fileName, char * outPath);

Preferences * Preferences_create(const char * identifier);
void Preferences_init(Preferences * self, const char * identifier);
void Preferences_dispose(Preferences * self);

void Preferences_addInteger(Preferences * self, const char * name, int defaultValue);
void Preferences_addFloat(Preferences * self, const char * name, float defaultValue);
void Preferences_addBoolean(Preferences * self, const char * name, bool defaultValue);
void Preferences_addString(Preferences * self, const char * name, const char * defaultValue);
void Preferences_addData(Preferences * self, const char * name, const void * defaultValue, size_t defaultLength);

int Preferences_getInteger(Preferences * self, const char * name);
float Preferences_getFloat(Preferences * self, const char * name);
bool Preferences_getBoolean(Preferences * self, const char * name);
const char * Preferences_getString(Preferences * self, const char * name);
void * Preferences_getData(Preferences * self, const char * name, size_t * outLength);

void Preferences_setInteger(Preferences * self, const char * name, int value);
void Preferences_setFloat(Preferences * self, const char * name, float value);
void Preferences_setBoolean(Preferences * self, const char * name, bool value);
void Preferences_setString(Preferences * self, const char * name, const char * value);
void Preferences_setData(Preferences * self, const char * name, const void * value, size_t length);

void Preferences_load(Preferences * self);
void Preferences_save(Preferences * self);
void Preferences_loadDefaultValues(Preferences * self);
void Preferences_loadDefaultValue(Preferences * self, const char * name);

#endif
