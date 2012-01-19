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

#ifndef __BINARY_SERIALIZATION_SHARED_H__
#define __BINARY_SERIALIZATION_SHARED_H__

// Data ended prematurely
#define BINARY_SERIALIZATION_ERROR_UNEXPECTED_EOF 200

// First four bytes of the file were neither 'Stem' (big endian) nor 'metS' (little endian)
#define BINARY_SERIALIZATION_ERROR_INVALID_SIGNATURE 201

// Top level container ends before EOF
#define BINARY_SERIALIZATION_ERROR_EXTRA_DATA_AT_EOF 202

// A call to endArray was made without first reading all elements in it
#define BINARY_SERIALIZATION_ERROR_ARRAY_NOT_FULLY_READ 203

// A dictionary offset was specified beyond EOF
#define BINARY_SERIALIZATION_ERROR_INVALID_OFFSET 204

enum BinarySerializationContainerType {
	BINARY_SERIALIZATION_CONTAINER_TYPE_STRUCT,
	BINARY_SERIALIZATION_CONTAINER_TYPE_ARRAY,
	BINARY_SERIALIZATION_CONTAINER_TYPE_DICTIONARY
};

#endif
