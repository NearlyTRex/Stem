/*
  Copyright (c) 2012 Alex Diener
  
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

#ifndef __JSON_SERIALIZATION_SHARED_H__
#define __JSON_SERIALIZATION_SHARED_H__

// The structure key being written was already used in the same structure
#define JSON_SERIALIZATION_ERROR_DUPLICATE_STRUCTURE_KEY 101

// Provided file/string couldn't be parsed as JSON
#define JSON_SERIALIZATION_ERROR_INVALID_JSON 102

// JSON array contains more subitems than number of bits in requested data type
#define JSON_SERIALIZATION_ERROR_TOO_MANY_BITS 103

// writeTo<String|File|JSONNode> called without having properly terminated data serialization
#define JSON_SERIALIZATION_ERROR_INCOMPLETE 104

// endArray called while there's still data to read
#define JSON_SERIALIZATION_ERROR_CONTAINER_NOT_FULLY_READ 105

enum JSONSerializationContainerType {
	JSON_SERIALIZATION_CONTAINER_TYPE_ARRAY,
	JSON_SERIALIZATION_CONTAINER_TYPE_STRUCTURE,
	JSON_SERIALIZATION_CONTAINER_TYPE_DICTIONARY
};

#endif
