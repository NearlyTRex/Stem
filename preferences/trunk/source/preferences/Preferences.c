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

void Preferences_getFilePath(const char * fileName, char * outPath) {
	Preferences_getFilePathPrivate(fileName, outPath);
}

Preferences * Preferences_create(const char * identifier) {
	stemobject_create_implementation(Preferences, init, identifier)
}

bool Preferences_init(Preferences * self, const char * identifier) {
	char * newIdentifier;
	
	call_super(init, self);
	
	self->valueCount = 0;
	self->values = NULL;
	newIdentifier = malloc(strlen(identifier) + 1);
	strcpy(newIdentifier, identifier);
	self->identifier = newIdentifier;
	self->eventDispatcher = EventDispatcher_create();
	
	self->dispose = Preferences_dispose;
	return true;
}

void Preferences_dispose(Preferences * self) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		free(self->values[valueIndex].name);
		if (self->values[valueIndex].type == PREFERENCES_TYPE_STRING) {
			free(self->values[valueIndex].value.string);
			free(self->values[valueIndex].defaultValue.string);
			
		} else if (self->values[valueIndex].type == PREFERENCES_TYPE_DATA) {
			free(self->values[valueIndex].value.data.bytes);
			free(self->values[valueIndex].defaultValue.data.bytes);
		}
	}
	free(self->values);
	free((char *) self->identifier);
	
	call_super(dispose, self);
}

void Preferences_addInteger(Preferences * self, const char * name, int defaultValue) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return;
		}
	}
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_INTEGER;
	self->values[self->valueCount].value.integer = defaultValue;
	self->values[self->valueCount].defaultValue.integer = defaultValue;
	self->valueCount++;
}

void Preferences_addFloat(Preferences * self, const char * name, float defaultValue) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return;
		}
	}
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_FLOAT;
	self->values[self->valueCount].value.number = defaultValue;
	self->values[self->valueCount].defaultValue.number = defaultValue;
	self->valueCount++;
}

void Preferences_addBoolean(Preferences * self, const char * name, bool defaultValue) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return;
		}
	}
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_BOOLEAN;
	self->values[self->valueCount].value.boolean = defaultValue;
	self->values[self->valueCount].defaultValue.boolean = defaultValue;
	self->valueCount++;
}

void Preferences_addString(Preferences * self, const char * name, const char * defaultValue) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return;
		}
	}
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_STRING;
	self->values[self->valueCount].value.string = malloc(strlen(defaultValue) + 1);
	strcpy(self->values[self->valueCount].value.string, defaultValue);
	self->values[self->valueCount].defaultValue.string = malloc(strlen(defaultValue) + 1);
	strcpy(self->values[self->valueCount].defaultValue.string, defaultValue);
	self->valueCount++;
}

void Preferences_addData(Preferences * self, const char * name, const void * defaultValue, size_t defaultLength) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return;
		}
	}
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_DATA;
	self->values[self->valueCount].value.data.length = defaultLength;
	self->values[self->valueCount].value.data.bytes = malloc(defaultLength);
	memcpy(self->values[self->valueCount].value.data.bytes, defaultValue, defaultLength);
	self->values[self->valueCount].defaultValue.data.length = defaultLength;
	self->values[self->valueCount].defaultValue.data.bytes = malloc(defaultLength);
	memcpy(self->values[self->valueCount].defaultValue.data.bytes, defaultValue, defaultLength);
	self->valueCount++;
}

int Preferences_getInteger(Preferences * self, const char * name) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_INTEGER && !strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.integer;
		}
	}
	return 0;
}

float Preferences_getFloat(Preferences * self, const char * name) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_FLOAT && !strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.number;
		}
	}
	return 0.0f;
}

bool Preferences_getBoolean(Preferences * self, const char * name) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_BOOLEAN && !strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.boolean;
		}
	}
	return false;
}

const char * Preferences_getString(Preferences * self, const char * name) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_STRING && !strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.string;
		}
	}
	return NULL;
}

void * Preferences_getData(Preferences * self, const char * name, size_t * outLength) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_DATA && !strcmp(self->values[valueIndex].name, name)) {
			if (outLength != NULL) {
				*outLength = self->values[valueIndex].value.data.length;
			}
			return self->values[valueIndex].value.data.bytes;
		}
	}
	return NULL;
}

void Preferences_setInteger(Preferences * self, const char * name, int value) {
	struct PreferencesEvent event;
	
	event.name = name;
	event.type = PREFERENCES_TYPE_INTEGER;
	event.value.integer = value;
	event.previousValue.integer = Preferences_getInteger(self, name);
	EventDispatcher_dispatchEvent(self->eventDispatcher, Atom_fromString(PREFERENCES_EVENT_VALUE_CHANGED), &event);
	
	Preferences_setIntegerPrivate(self, name, event.value.integer);
}

void Preferences_setFloat(Preferences * self, const char * name, float value) {
	struct PreferencesEvent event;
	
	event.name = name;
	event.type = PREFERENCES_TYPE_FLOAT;
	event.value.number = value;
	event.previousValue.number = Preferences_getFloat(self, name);
	EventDispatcher_dispatchEvent(self->eventDispatcher, Atom_fromString(PREFERENCES_EVENT_VALUE_CHANGED), &event);
	
	Preferences_setFloatPrivate(self, name, event.value.number);
}

void Preferences_setBoolean(Preferences * self, const char * name, bool value) {
	struct PreferencesEvent event;
	
	event.name = name;
	event.type = PREFERENCES_TYPE_BOOLEAN;
	event.value.boolean = value;
	event.previousValue.boolean = Preferences_getBoolean(self, name);
	EventDispatcher_dispatchEvent(self->eventDispatcher, Atom_fromString(PREFERENCES_EVENT_VALUE_CHANGED), &event);
	
	Preferences_setBooleanPrivate(self, name, event.value.boolean);
}

void Preferences_setString(Preferences * self, const char * name, const char * value) {
	struct PreferencesEvent event;
	
	event.name = name;
	event.type = PREFERENCES_TYPE_STRING;
	event.value.string = value;
	event.previousValue.string = Preferences_getString(self, name);
	EventDispatcher_dispatchEvent(self->eventDispatcher, Atom_fromString(PREFERENCES_EVENT_VALUE_CHANGED), &event);
	
	Preferences_setStringPrivate(self, name, event.value.string);
}

void Preferences_setData(Preferences * self, const char * name, const void * value, size_t length) {
	struct PreferencesEvent event;
	
	event.name = name;
	event.type = PREFERENCES_TYPE_DATA;
	event.value.data.bytes = value;
	event.value.data.length = length;
	event.previousValue.data.bytes = Preferences_getData(self, name, &event.previousValue.data.length);
	EventDispatcher_dispatchEvent(self->eventDispatcher, Atom_fromString(PREFERENCES_EVENT_VALUE_CHANGED), &event);
	
	Preferences_setDataPrivate(self, name, event.value.data.bytes, event.value.data.length);
}

void Preferences_load(Preferences * self) {
	Preferences_loadPrivate(self);
}

void Preferences_save(Preferences * self) {
	Preferences_savePrivate(self);
}

void Preferences_loadDefaultValues(Preferences * self) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		Preferences_loadDefaultValue(self, self->values[valueIndex].name);
	}
}

void Preferences_loadDefaultValue(Preferences * self, const char * name) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			switch (self->values[valueIndex].type) {
				case PREFERENCES_TYPE_INTEGER:
					self->values[valueIndex].value.integer = self->values[valueIndex].defaultValue.integer;
					break;
					
				case PREFERENCES_TYPE_FLOAT:
					self->values[valueIndex].value.number = self->values[valueIndex].defaultValue.number;
					break;
					
				case PREFERENCES_TYPE_BOOLEAN:
					self->values[valueIndex].value.boolean = self->values[valueIndex].defaultValue.boolean;
					break;
					
				case PREFERENCES_TYPE_STRING:
					free(self->values[valueIndex].value.string);
					self->values[valueIndex].value.string = malloc(strlen(self->values[valueIndex].defaultValue.string) + 1);
					strcpy(self->values[valueIndex].value.string, self->values[valueIndex].defaultValue.string);
					break;
					
				case PREFERENCES_TYPE_DATA:
					self->values[valueIndex].value.data.length = self->values[valueIndex].defaultValue.data.length;
					free(self->values[valueIndex].value.data.bytes);
					self->values[valueIndex].value.data.bytes = malloc(self->values[valueIndex].defaultValue.data.length);
					memcpy(self->values[valueIndex].value.data.bytes, self->values[valueIndex].defaultValue.data.bytes, self->values[valueIndex].defaultValue.data.length);
					break;
			}
			break;
		}
	}
}
