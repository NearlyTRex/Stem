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

#ifndef __DataArray_H__
#define __DataArray_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DataArray DataArray;

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dynamictypes/DataValue.h"

struct DataArray {
	size_t count;
	size_t allocatedCount;
	DataValue * values;
};

#define ARRAY_VALUE_SENTINEL ((DataValue) {{.boolean = false}, -1, false})

/** Returns an empty array. */
DataArray * arrayCreate();

/** Returns an array initialized with the specified values. Use the arrayCreateWithValues macro instead of calling _arrayCreateWithValues directly. */
DataArray * _arrayCreateWithValues(DataValue value, ...);
#define arrayCreateWithValues(...) _arrayCreateWithValues(__VA_ARGS__, ARRAY_VALUE_SENTINEL)

/** Returns a deep copy of an array. */
DataArray * arrayCopy(DataArray * array);

/** Disposes the array and all entries in it over which the array has ownership. */
void arrayDispose(DataArray * array);

/** Removes the entry at the specified index. If the index is beyond the end of the array, this function has no effect. */
void arrayDelete(DataArray * array, size_t index);

/** Return a pointer to the DataValue at the specified index, or NULL if index is beyond the end of the array.
    Note that modifying the array may invalidate this pointer, so be sure to make a local copy of its contents
    if you need to cache it. */
DataValue * arrayGet(DataArray * array, size_t index);

/** Appends a value to the end of the array. */
void arrayAppend(DataArray * array, DataValue value);

/** Inserts a value into the array at the specified index, moving all entries forward to make room.
    If index is beyond the end of the array by more than 1, this function has no effect.
    If index is equal to the array length, this function behaves the same as arrayAppend. */
void arrayInsert(DataArray * array, size_t index, DataValue value);

/** Replaces the value at the specified index in the array. If index is beyond the end of the array, this function has no effect. */
void arrayReplace(DataArray * array, size_t index, DataValue value);

#ifdef __cplusplus
}
#endif
#endif
