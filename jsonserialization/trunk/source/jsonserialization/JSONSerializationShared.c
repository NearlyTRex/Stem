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

#include "jsonserialization/JSONSerializationShared.h"
#include "serialization/SerializationShared.h"

const char * JSONSerialization_errorString(int status) {
	switch (status) {
		case JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY:
			return "The structure key being written was already used in the same structure";
			
		case JSON_SERIALIZATION_ERROR_INVALID_JSON:
			return "Provided file/string couldn't be parsed as JSON";
			
		case JSON_SERIALIZATION_ERROR_TOO_MANY_BITS:
			return "JSON array contains more subitems than number of bits in requested data type";
			
		case JSON_SERIALIZATION_ERROR_INCOMPLETE:
			return "writeTo<String|File|JSONNode> called without having properly terminated data serialization";
			
		case JSON_SERIALIZATION_ERROR_CONTAINER_NOT_FULLY_READ:
			return "endArray called while there's still data to read";
						
		default:
			return Serialization_errorString(status);
	}
}
