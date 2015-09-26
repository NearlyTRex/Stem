/*
  Copyright (c) 2015 Alex Diener
  
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

#include "binaryserialization/BinarySerializationShared.h"
#include "serialization/SerializationShared.h"

const char * BinarySerialization_errorString(int status) {
	switch (status) {
		case BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF:
			return "Data ended prematurely";
			
		case BINARY_SERIALIZATION_ERROR_INVALID_SIGNATURE:
			return "First four bytes of the file were neither 'Stem' (big endian) nor 'metS' (little endian)";
			
		case BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF:
			return "Top level container ends before EOF";
			
		case BINARY_SERIALIZATION_ERROR_ARRAY_NOT_FULLY_READ:
			return "A call to endArray was made without first reading all elements in it";
			
		case BINARY_SERIALIZATION_ERROR_INVALID_OFFSET:
			return "A dictionary offset was specified beyond EOF";
			
		case BINARY_SERIALIZATION_ERROR_NO_TOP_LEVEL_CONTAINER:
			return "writeTo<Bytes|File> called without any data having been serialized";
			
		default:
			return Serialization_errorString(status);
	}
}
