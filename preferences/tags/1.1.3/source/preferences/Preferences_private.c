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

#include "preferences/Preferences_private.h"
#include <string.h>

void Preferences_setIntegerPrivate(Preferences * self, const char * name, int value) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_INTEGER && !strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.integer = value;
			break;
		}
	}
}

void Preferences_setFloatPrivate(Preferences * self, const char * name, float value) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_FLOAT && !strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.number = value;
			break;
		}
	}
}

void Preferences_setBooleanPrivate(Preferences * self, const char * name, bool value) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_BOOLEAN && !strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.boolean = value;
			break;
		}
	}
}

void Preferences_setStringPrivate(Preferences * self, const char * name, const char * value) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_STRING && !strcmp(self->values[valueIndex].name, name)) {
			free(self->values[valueIndex].value.string);
			self->values[valueIndex].value.string = malloc(strlen(value) + 1);
			strcpy(self->values[valueIndex].value.string, value);
			break;
		}
	}
}

void Preferences_setDataPrivate(Preferences * self, const char * name, const void * value, size_t length) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < self->valueCount; valueIndex++) {
		if (self->values[valueIndex].type == PREFERENCES_TYPE_DATA && !strcmp(self->values[valueIndex].name, name)) {
			self->values[valueIndex].value.data.length = length;
			free(self->values[valueIndex].value.data.bytes);
			self->values[valueIndex].value.data.bytes = malloc(length);
			memcpy(self->values[valueIndex].value.data.bytes, value, length);
			break;
		}
	}
}
