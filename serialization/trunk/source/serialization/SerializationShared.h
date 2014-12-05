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

#ifndef __SerializationShared_H__
#define __SerializationShared_H__
#ifdef __cplusplus
extern "C" {
#endif

#define enumKV(value) #value, value

// clang has a bug that doesn't allow sentinel to be applied to function pointers in structs
// http://llvm.org/bugs/show_bug.cgi?id=11988
#ifdef __clang__
#define struct_fptr_sentinel
#else
#define struct_fptr_sentinel __attribute((sentinel))
#endif

// No error
#define SERIALIZATION_ERROR_OK 0

// General incorrect API usage
#define SERIALIZATION_ERROR_INVALID_OPERATION 1

// The requested key is not present in the current dictionary or structure
#define SERIALIZATION_ERROR_KEY_NOT_FOUND 2

// A call to begin<Array|Structure|Dictionary> wasn't paired with the correct one of end<Array|Structure|Dictionary>
#define SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH 3

// More calls to end<Array|Structure|Dictionary> than to begin<Array|Structure|Dictionary>
#define SERIALIZATION_ERROR_CONTAINER_UNDERFLOW 4

// After a matched number of calls to <begin|end><Array|Structure|Dictionary>, begin<Array|Structure|Dictionary> is called again
#define SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS 5

// In a call to <read|write>Bitfield<8|16|32|64>, one or more of set bits isn't named in function arguments
#define SERIALIZATION_ERROR_UNNAMED_BIT 6

// In a call to <read|write>Bitfield<8|16|32|64>, multiple bits are given the same name
#define SERIALIZATION_ERROR_DUPLICATE_BIT 7

// In a call to <read|write>Enumeration, the enumeration value is not named
#define SERIALIZATION_ERROR_ENUM_NOT_NAMED 8

// In a call to <read|write>Enumeration, multiple enumeration values are given the same name
#define SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME 9

// In a call to <read|write>Enumeration, multiple enumeration names are given the same value
#define SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE 10

// key parameter is NULL when reading from/writing to a structure or dictionary
#define SERIALIZATION_ERROR_NULL_KEY 11

// The value being deserialized is not of the requested data type
#define SERIALIZATION_ERROR_INCORRECT_TYPE 12

// The number value being deserialized is too large or small to fit in the requested data type
#define SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE 13

// More fields requested for deserialization than exist in the container being deserialized
#define SERIALIZATION_ERROR_END_OF_CONTAINER 14

// A read or write call was issued before the first begin<Array|Structure|Dictionary> call, or after the last end<Array|Structure|Dictionary> call
#define SERIALIZATION_ERROR_NO_CONTAINER_STARTED 15

// An attempt was made to save data without having serialized anything
#define SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER 16

// Other errors defined by concrete serialization modules

/* Returns a human-readable string constant describing one of the errors defined above.
   Note that you should normally call the errorString() instance method on the context that
   generated the error instead of this function, since they can describe error codes defined
   by extensions. This function can only define the ones contained within this header. */
const char * Serialization_errorString(int status);

#ifdef __cplusplus
}
#endif
#endif
