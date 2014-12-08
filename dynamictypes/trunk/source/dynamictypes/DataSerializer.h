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

#ifndef __DataSerializer_H__
#define __DataSerializer_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "dynamictypes/DataValue.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"

// Reads from or writes to a lossless representation of a DataValue hierarchy. Restrictions:
// - value must be a container type (DATA_TYPE_ARRAY, DATA_TYPE_HASH_TABLE, or DATA_TYPE_ASSOCIATIVE_ARRAY)
// - Values of type DATA_TYPE_POINTER anywhere in the hierarchy will be ignored and will not be serialized

void DataValue_serialize(DataValue * value, compat_type(SerializationContext *) serializationContext);

// If deserialization fails, returns valueCreateBoolean(false)
DataValue DataValue_deserialize(compat_type(DeserializationContext *) deserializationContext);

#ifdef __cplusplus
}
#endif
#endif
