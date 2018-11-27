/*
  Copyright (c) 2018 Alex Diener
  
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

#ifndef __TextureAtlasData_H__
#define __TextureAtlasData_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TextureAtlasData TextureAtlasData;

#include "glgraphics/TextureAtlas.h"
#include "glgraphics/VertexTypes.h"
#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include "utilities/Atom.h"

#define TEXTUREATLASDATA_FORMAT_VERSION 2
#define TEXTUREATLASDATA_FORMAT_TYPE "texture_atlas"

struct TextureAtlasData_entry {
	Atom name;
	float left;
	float right;
	float bottom;
	float top;
};

#define TextureAtlasData_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	unsigned int entryCount; \
	struct TextureAtlasData_entry * entries; \
	Atom name; \
	Atom textureMapName; \
	bool magnifyNearest;

stemobject_struct_definition(TextureAtlasData)

TextureAtlasData * TextureAtlasData_create(Atom name, Atom textureMapName, bool magnifyNearest, unsigned int entryCount, struct TextureAtlasData_entry * entries);
bool TextureAtlasData_init(TextureAtlasData * self, Atom name, Atom textureMapName, bool magnifyNearest, unsigned int entryCount, struct TextureAtlasData_entry * entries);
void TextureAtlasData_dispose(TextureAtlasData * self);

TextureAtlasData * TextureAtlasData_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool TextureAtlasData_loadSerializedData(TextureAtlasData * self, compat_type(DeserializationContext *) deserializationContext);
void TextureAtlasData_serialize(TextureAtlasData * self, compat_type(SerializationContext *) serializationContext);

// Creates a TextureAtlas object initialized with this TextureAtlasData's entries.
// Does not call TextureAtlas_setTexture(); the caller must fetch the texture image themselves and call this function
// to fully initialize the returned TextureAtlas.
TextureAtlas * TextureAtlasData_createTextureAtlas(TextureAtlasData * self);

#ifdef __cplusplus
}
#endif
#endif
