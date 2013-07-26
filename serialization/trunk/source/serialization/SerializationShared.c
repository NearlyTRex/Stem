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

#include "serialization/SerializationShared.h"

const char * Serialization_errorString(int status) {
	switch (status) {
		case SERIALIZATION_ERROR_OK:
			return "No error";
			
		case SERIALIZATION_ERROR_INVALID_OPERATION:
			return "General incorrect API usage";
			
		case SERIALIZATION_ERROR_KEY_NOT_FOUND:
			return "The requested key is not present in the current dictionary or structure";
			
		case SERIALIZATION_ERROR_CONTAINER_TYPE_MISMATCH:
			return "A call to begin<Array|Structure|Dictionary> wasn't paired with the correct one of end<Array|Structure|Dictionary>";
			
		case SERIALIZATION_ERROR_CONTAINER_UNDERFLOW:
			return "More calls to end<Array|Structure|Dictionary> than to begin<Array|Structure|Dictionary>";
			
		case SERIALIZATION_ERROR_MULTIPLE_TOP_LEVEL_CONTAINERS:
			return "After a matched number of calls to <begin|end><Array|Structure|Dictionary>, begin<Array|Structure|Dictionary> is called again";
			
		case SERIALIZATION_ERROR_UNNAMED_BIT:
			return "In a call to <read|write>Bitfield<8|16|32|64>, one or more of set bits isn't named in function arguments";
			
		case SERIALIZATION_ERROR_DUPLICATE_BIT:
			return "In a call to <read|write>Bitfield<8|16|32|64>, multiple bits are given the same name";
			
		case SERIALIZATION_ERROR_ENUM_NOT_NAMED:
			return "In a call to <read|write>Enumeration, the enumeration value is not named";
			
		case SERIALIZATION_ERROR_DUPLICATE_ENUM_NAME:
			return "In a call to <read|write>Enumeration, multiple enumeration values are given the same name";
			
		case SERIALIZATION_ERROR_DUPLICATE_ENUM_VALUE:
			return "In a call to <read|write>Enumeration, multiple enumeration names are given the same value";
			
		case SERIALIZATION_ERROR_NULL_KEY:
			return "key parameter is NULL when reading from/writing to a structure or dictionary";
			
		case SERIALIZATION_ERROR_INCORRECT_TYPE:
			return "The value being deserialized is not of the requested data type";
			
		case SERIALIZATION_ERROR_NUMBER_OUT_OF_RANGE:
			return "The number value being deserialized is too large or small to fit in the requested data type";
			
		case SERIALIZATION_ERROR_END_OF_CONTAINER:
			return "More fields requested for deserialization than exist in the container being deserialized";
			
		case SERIALIZATION_ERROR_NO_CONTAINER_STARTED:
			return "A read or write call was issued before the first begin<Array|Structure|Dictionary> call, or after the last end<Array|Structure|Dictionary> call";
			
		case SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER:
			return "An attempt was made to save data without having serialized anything";
			
		default:
			return "Unknown error";
	}
}
