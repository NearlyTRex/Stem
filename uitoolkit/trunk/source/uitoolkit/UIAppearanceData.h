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

#ifndef __UIAppearanceData_H__
#define __UIAppearanceData_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct UIAppearanceData UIAppearanceData;

#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include "uitoolkit/UIAppearance.h"
#include "utilities/Atom.h"

#define UIAPPEARANCEDATA_FORMAT_VERSION 1
#define UIAPPEARANCEDATA_FORMAT_TYPE "ui_appearance"

#define UIAppearanceData_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	Atom name; \
	Atom atlasName; \
	Atom fontName; \
	struct UIAppearance_metrics metrics;

stemobject_struct_definition(UIAppearanceData)

UIAppearanceData * UIAppearanceData_create(Atom name, Atom atlasName, Atom fontName, struct UIAppearance_metrics metrics);
bool UIAppearanceData_init(UIAppearanceData * self, Atom name, Atom atlasName, Atom fontName, struct UIAppearance_metrics metrics);
void UIAppearanceData_dispose(UIAppearanceData * self);

UIAppearanceData * UIAppearanceData_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool UIAppearanceData_loadSerializedData(UIAppearanceData * self, compat_type(DeserializationContext *) deserializationContext);
void UIAppearanceData_serialize(UIAppearanceData * self, compat_type(SerializationContext *) serializationContext);

// Creates a UIAppearance object initialized with this UIAppearanceData's metrics, and the provided font and texture atlas.
UIAppearance * UIAppearanceData_createUIAppearance(UIAppearanceData * self, TextureAtlas * atlas, GLBitmapFont * font);

#ifdef __cplusplus
}
#endif
#endif
