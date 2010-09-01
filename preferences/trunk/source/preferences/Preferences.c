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

#include "preferences/Preferences.h"
#include "preferences/Preferences_private.h"
#include <string.h>

Preferences * Preferences_create(const char * identifier, PreferencesDomain domain) {
	stemobject_create_implementation(Preferences, init, identifier, domain)
}

void Preferences_init(compat_type(Preferences *) selfPtr, const char * identifier, PreferencesDomain domain) {
	Preferences * self = selfPtr;
	
	StemObject_init(self);
	
	self->valueCount = 0;
	self->values = NULL;
	
	self->dispose = Preferences_dispose;
	self->addInteger = Preferences_addInteger;
	self->addFloat = Preferences_addFloat;
	self->addBoolean = Preferences_addBoolean;
	self->addString = Preferences_addString;
	self->addData = Preferences_addData;
	self->getInteger = Preferences_getInteger;
	self->getFloat = Preferences_getFloat;
	self->getBoolean = Preferences_getBoolean;
	self->getString = Preferences_getString;
	self->getData = Preferences_getData;
	self->setInteger = Preferences_setInteger;
	self->setFloat = Preferences_setFloat;
	self->setBoolean = Preferences_setBoolean;
	self->setString = Preferences_setString;
	self->setData = Preferences_setData;
	self->load = Preferences_load;
	self->save = Preferences_save;
	self->loadDefaultValues = Preferences_loadDefaultValues;
	self->loadDefaultValue = Preferences_loadDefaultValue;
}

void Preferences_dispose(compat_type(Preferences *) selfPtr) {
	Preferences * self = selfPtr;
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
	
	StemObject_dispose(selfPtr);
}

void Preferences_addInteger(compat_type(Preferences *) selfPtr, const char * name, int defaultValue) {
	Preferences * self = selfPtr;
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_INTEGER;
	self->values[self->valueCount].value.integer = defaultValue;
	self->values[self->valueCount].defaultValue.integer = defaultValue;
	self->valueCount++;
}

void Preferences_addFloat(compat_type(Preferences *) selfPtr, const char * name, float defaultValue) {
	Preferences * self = selfPtr;
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_FLOAT;
	self->values[self->valueCount].value.number = defaultValue;
	self->values[self->valueCount].defaultValue.number = defaultValue;
	self->valueCount++;
}

void Preferences_addBoolean(compat_type(Preferences *) selfPtr, const char * name, bool defaultValue) {
	Preferences * self = selfPtr;
	
	self->values = realloc(self->values, sizeof(struct Preferences_valueRecord) * (self->valueCount + 1));
	self->values[self->valueCount].name = malloc(strlen(name) + 1);
	strcpy(self->values[self->valueCount].name, name);
	self->values[self->valueCount].type = PREFERENCES_TYPE_BOOLEAN;
	self->values[self->valueCount].value.boolean = defaultValue;
	self->values[self->valueCount].defaultValue.boolean = defaultValue;
	self->valueCount++;
}

void Preferences_addString(compat_type(Preferences *) selfPtr, const char * name, const char * defaultValue) {
	Preferences * self = selfPtr;
	
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

void Preferences_addData(compat_type(Preferences *) selfPtr, const char * name, void * defaultValue, size_t defaultLength) {
	Preferences * self = selfPtr;
	
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

int Preferences_getInteger(compat_type(Preferences *) selfPtr, const char * name) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.integer;
		}
	}
	return 0;
}

float Preferences_getFloat(compat_type(Preferences *) selfPtr, const char * name) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.number;
		}
	}
	return 0.0f;
}

bool Preferences_getBoolean(compat_type(Preferences *) selfPtr, const char * name) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.boolean;
		}
	}
	return false;
}

const char * Preferences_getString(compat_type(Preferences *) selfPtr, const char * name) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			return self->values[valueIndex].value.string;
		}
	}
	return NULL;
}

void * Preferences_getData(compat_type(Preferences *) selfPtr, const char * name, size_t * outLength) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			if (outLength != NULL) {
				*outLength = self->values[valueIndex].value.data.length;
			}
			return self->values[valueIndex].value.data.bytes;
		}
	}
	return NULL;
}

void Preferences_setInteger(compat_type(Preferences *) selfPtr, const char * name, int value) {
}

void Preferences_setFloat(compat_type(Preferences *) selfPtr, const char * name, float value) {
}

void Preferences_setBoolean(compat_type(Preferences *) selfPtr, const char * name, bool value) {
}

void Preferences_setString(compat_type(Preferences *) selfPtr, const char * name, const char * value) {
}

void Preferences_setData(compat_type(Preferences *) selfPtr, const char * name, void * value, size_t length) {
}

void Preferences_load(compat_type(Preferences *) selfPtr) {
	Preferences_loadPrivate(selfPtr);
}

void Preferences_save(compat_type(Preferences *) selfPtr) {
}

void Preferences_loadDefaultValues(compat_type(Preferences *) selfPtr) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		self->loadDefaultValue(self, self->values[valueIndex].name);
	}
}

void Preferences_loadDefaultValue(compat_type(Preferences *) selfPtr, const char * name) {
	Preferences * self = selfPtr;
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

void Preferences_setIntegerPrivate(compat_type(Preferences *) selfPtr, const char * name, int value) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.integer = value;
			break;
		}
	}
}

void Preferences_setFloatPrivate(compat_type(Preferences *) selfPtr, const char * name, float value) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.number = value;
			break;
		}
	}
}

void Preferences_setBooleanPrivate(compat_type(Preferences *) selfPtr, const char * name, bool value) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.boolean = value;
			break;
		}
	}
}

void Preferences_setStringPrivate(compat_type(Preferences *) selfPtr, const char * name, const char * value) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			free(self->values[valueIndex].value.string);
			self->values[valueIndex].value.string = malloc(strlen(value) + 1);
			strcpy(self->values[valueIndex].value.string, value);
			break;
		}
	}
}

void Preferences_setDataPrivate(compat_type(Preferences *) selfPtr, const char * name, void * value, size_t length) {
	Preferences * self = selfPtr;
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (!strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.data.length = length;
			free(self->values[valueIndex].value.data.bytes);
			self->values[valueIndex].value.data.bytes = malloc(length);
			memcpy(self->values[valueIndex].value.data.bytes, value, length);
			break;
		}
	}
}
