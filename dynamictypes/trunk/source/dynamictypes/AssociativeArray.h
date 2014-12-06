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

#ifndef __AssociativeArray_H__
#define __AssociativeArray_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct AssociativeArray AssociativeArray;

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dynamictypes/DataValue.h"

struct AssociativeArray {
	size_t count;
	size_t allocatedCount;
	const char ** keys;
	DataValue * values;
};

#define ASSOCIATIVE_ARRAY_NOT_FOUND ((size_t) -1)

/** Returns an empty associative array. */
AssociativeArray * associativeArrayCreate();

/** Returns an associative array initialized with the specified keys and values. */
AssociativeArray * associativeArrayCreateWithKeysAndValues(const char * key, DataValue value, ...) __attribute__((sentinel));

/** Returns a deep copy of an associative array. */
AssociativeArray * associativeArrayCopy(AssociativeArray * array);

/** Disposes the associative array and all entries in it over which the associative array has ownership. */
void associativeArrayDispose(AssociativeArray * array);

/** Removes the entry at the specified index. If the index is beyond the end of the array, this function has no effect. */
void associativeArrayDelete(AssociativeArray * array, size_t index);

/** Return a pointer to the key at the specified index, or NULL if index is beyond the end of the array.
    Note that modifying the array may invalidate this pointer, so be sure to make a local copy of its contents
    if you need to cache it. */
const char * associativeArrayGetKeyAtIndex(AssociativeArray * array, size_t index);

/** Return a pointer to the DataValue at the specified index, or NULL if index is beyond the end of the array.
    Note that modifying the array may invalidate this pointer, so be sure to make a local copy of its contents
    if you need to cache it. */
DataValue * associativeArrayGetValueAtIndex(AssociativeArray * array, size_t index);

/** Returns the index of the first item in the array that matches key, or ASSOCIATIVE_ARRAY_NOT_FOUND if no item matches it. */
size_t associativeArrayGetFirstIndexOfKey(AssociativeArray * array, const char * key);

/** Returns the value of the first item in the array that matches key, or NULL if no item matches it. */
DataValue * associativeArrayGetFirstValueForKey(AssociativeArray * array, const char * key);

/** Appends a key/value pair to the end of the array. */
void associativeArrayAppend(AssociativeArray * array, const char * key, DataValue value);

/** Inserts a key/value pair into the array at the specified index, moving all entries forward to make room.
    If index is beyond the end of the array by more than 1, this function has no effect.
    If index is equal to the array length, this function behaves the same as arrayAppend. */
void associativeArrayInsert(AssociativeArray * array, size_t index, const char * key, DataValue value);

/** Replaces the key/value pair at the specified index in the array. If index is beyond the end of the array, this function has no effect. */
void associativeArrayReplace(AssociativeArray * array, size_t index, const char * key, DataValue value);

#ifdef __cplusplus
}
#endif
#endif
