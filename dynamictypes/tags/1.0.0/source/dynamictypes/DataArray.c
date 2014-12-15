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

#include "dynamictypes/DataArray.h"
#include <stdarg.h>

DataArray * arrayCreate() {
	DataArray * array;
	
	array = malloc(sizeof(DataArray));
	array->count = 0;
	array->allocatedCount = 0;
	array->values = NULL;
	return array;
}

DataArray * _arrayCreateWithValues(DataValue value, ...) {
	DataArray * array;
	va_list args;
	
	array = arrayCreate();
	arrayAppend(array, value);
	va_start(args, value);
	while ((value = va_arg(args, DataValue)).type != ARRAY_VALUE_SENTINEL.type) {
		arrayAppend(array, value);
	}
	return array;
}

DataArray * arrayCopy(DataArray * array) {
	DataArray * copy;
	size_t valueIndex;
	
	copy = arrayCreate();
	for (valueIndex = 0; valueIndex < array->count; valueIndex++) {
		arrayAppend(copy, valueCopy(&array->values[valueIndex]));
	}
	return copy;
}

void arrayDispose(DataArray * array) {
	size_t valueIndex;
	
	for (valueIndex = 0; valueIndex < array->count; valueIndex++) {
		valueDispose(&array->values[valueIndex]);
	}
	free(array->values);
	free(array);
}

void arrayDelete(DataArray * array, size_t index) {
	size_t valueIndex;
	
	if (index > array->count) {
		return;
	}
	valueDispose(&array->values[index]);
	for (valueIndex = index; valueIndex < array->count; valueIndex++) {
		array->values[valueIndex] = array->values[valueIndex + 1];
	}
	array->count--;
}

DataValue * arrayGet(DataArray * array, size_t index) {
	if (index >= array->count) {
		return NULL;
	}
	return &array->values[index];
}

static void resizeArray(DataArray * array) {
	if (array->allocatedCount <= array->count) {
		if (array->allocatedCount == 0) {
			array->allocatedCount = 1;
		} else {
			array->allocatedCount *= 2;
		}
		array->values = realloc(array->values, sizeof(DataValue) * array->allocatedCount);
	}
}

void arrayAppend(DataArray * array, DataValue value) {
	resizeArray(array);
	array->values[array->count++] = value;
}

void arrayInsert(DataArray * array, size_t index, DataValue value) {
	size_t valueIndex;
	
	if (index > array->count) {
		return;
	}
	resizeArray(array);
	for (valueIndex = array->count; valueIndex > index; valueIndex--) {
		array->values[valueIndex] = array->values[valueIndex - 1];
	}
	array->values[valueIndex] = value;
	array->count++;
}

void arrayReplace(DataArray * array, size_t index, DataValue value) {
	if (index >= array->count) {
		return;
	}
	valueDispose(&array->values[index]);
	array->values[index] = value;
}
