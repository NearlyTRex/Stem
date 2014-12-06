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

#include "dynamictypes/AssociativeArray.h"
#include <stdarg.h>
#include <string.h>

AssociativeArray * associativeArrayCreate() {
	AssociativeArray * array;
	
	array = malloc(sizeof(AssociativeArray));
	array->count = 0;
	array->allocatedCount = 0;
	array->values = NULL;
	array->keys = NULL;
	return array;
}

AssociativeArray * associativeArrayCreateWithKeysAndValues(const char * key, DataValue value, ...) {
	AssociativeArray * array;
	va_list args;
	
	array = associativeArrayCreate();
	associativeArrayAppend(array, key, value);
	va_start(args, value);
	while ((key = va_arg(args, const char *)) != NULL) {
		value = va_arg(args, DataValue);
		associativeArrayAppend(array, key, value);
	}
	return array;
}

AssociativeArray * associativeArrayCopy(AssociativeArray * array) {
	AssociativeArray * copy;
	size_t valueIndex;
	
	copy = associativeArrayCreate();
	for (valueIndex = 0; valueIndex < array->count; valueIndex++) {
		associativeArrayAppend(copy, array->keys[valueIndex], valueCopy(&array->values[valueIndex]));
	}
	return copy;
}

void associativeArrayDispose(AssociativeArray * array) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < array->count; valueIndex++) {
		valueDispose(&array->values[valueIndex]);
		free((void *) array->keys[valueIndex]);
	}
	free(array->values);
	free(array->keys);
	free(array);
}

void associativeArrayDelete(AssociativeArray * array, size_t index) {
	size_t valueIndex;
	
	if (index > array->count) {
		return;
	}
	valueDispose(&array->values[index]);
	free((void *) array->keys[index]);
	for (valueIndex = index; valueIndex < array->count; valueIndex++) {
		array->values[valueIndex] = array->values[valueIndex + 1];
		array->keys[valueIndex] = array->keys[valueIndex + 1];
	}
	array->count--;
}

const char * associativeArrayGetKeyAtIndex(AssociativeArray * array, size_t index) {
	if (index >= array->count) {
		return NULL;
	}
	return array->keys[index];
}

DataValue * associativeArrayGetValueAtIndex(AssociativeArray * array, size_t index) {
	if (index >= array->count) {
		return NULL;
	}
	return &array->values[index];
}

size_t associativeArrayGetFirstIndexOfKey(AssociativeArray * array, const char * key) {
	size_t index;
	
	for (index = 0; index < array->count; index++) {
		if (!strcmp(array->keys[index], key)) {
			return index;
		}
	}
	return ASSOCIATIVE_ARRAY_NOT_FOUND;
}

DataValue * associativeArrayGetFirstValueForKey(AssociativeArray * array, const char * key) {
	size_t index;
	
	for (index = 0; index < array->count; index++) {
		if (!strcmp(array->keys[index], key)) {
			return &array->values[index];
		}
	}
	return NULL;
}

static void resizeArray(AssociativeArray * array) {
	if (array->allocatedCount <= array->count) {
		if (array->allocatedCount == 0) {
			array->allocatedCount = 1;
		} else {
			array->allocatedCount *= 2;
		}
		array->values = realloc(array->values, sizeof(DataValue) * array->allocatedCount);
		array->keys = realloc(array->keys, sizeof(const char *) * array->allocatedCount);
	}
}

void associativeArrayAppend(AssociativeArray * array, const char * key, DataValue value) {
	resizeArray(array);
	array->values[array->count] = value;
	array->keys[array->count] = strdup(key);
	array->count++;
}

void associativeArrayInsert(AssociativeArray * array, size_t index, const char * key, DataValue value) {
	size_t valueIndex;
	
	if (index > array->count) {
		return;
	}
	resizeArray(array);
	for (valueIndex = array->count; valueIndex > index; valueIndex--) {
		array->values[valueIndex] = array->values[valueIndex - 1];
		array->keys[valueIndex] = array->keys[valueIndex - 1];
	}
	array->values[valueIndex] = value;
	array->keys[valueIndex] = strdup(key);
	array->count++;
}

void associativeArrayReplace(AssociativeArray * array, size_t index, const char * key, DataValue value) {
	if (index >= array->count) {
		return;
	}
	valueDispose(&array->values[index]);
	free((void *) array->keys[index]);
	array->values[index] = value;
	array->keys[index] = strdup(key);
}
