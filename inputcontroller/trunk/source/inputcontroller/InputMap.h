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

#ifndef __InputMap_H__
#define __InputMap_H__
#ifdef __cplusplus
extern "c" {
#endif

typedef struct InputMap InputMap;

#include "serialization/DeserializationContext.h"
#include "serialization/SerializationContext.h"
#include "stemobject/StemObject.h"
#include "utilities/Atom.h"
#include <stdlib.h>

#define INPUT_MAP_SERIALIZATION_FORMAT_VERSION 2

struct InputMap_keyboardBinding {
	Atom actionID;
	unsigned int keyCode;
	unsigned int charCode;
};

struct InputMap_keyModifierBinding {
	Atom actionID;
	int modifierBit;
};

struct InputMap_gamepadButtonBinding {
	Atom actionID;
	unsigned int buttonID;
};

struct InputMap_gamepadAxisBinding {
	Atom actionID;
	unsigned int axisID;
	float triggerThreshold;
	float releaseThreshold;
};

struct InputMap_gamepadMap {
	int vendorID;
	int productID;
	unsigned int buttonBindingCount;
	struct InputMap_gamepadButtonBinding * buttonBindings;
	unsigned int axisBindingCount;
	struct InputMap_gamepadAxisBinding * axisBindings;
};

#define InputMap_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	unsigned int keyboardBindingCount; \
	struct InputMap_keyboardBinding * keyboardBindings; \
	unsigned int keyModifierBindingCount; \
	struct InputMap_keyModifierBinding * keyModifierBindings; \
	unsigned int gamepadMapCount; \
	struct InputMap_gamepadMap * gamepadMaps;

stemobject_struct_definition(InputMap)

InputMap * InputMap_create();
bool InputMap_init(InputMap * self);
void InputMap_dispose(InputMap * self);

InputMap * InputMap_deserialize(compat_type(DeserializationContext *) deserializationContext);
bool InputMap_loadSerializedData(InputMap * self, compat_type(DeserializationContext *) deserializationContext);
void InputMap_serialize(InputMap * self, compat_type(SerializationContext *) serializationContext);

bool InputMap_isKeyBound(InputMap * self, Atom actionID, unsigned int keyCode);
void InputMap_bindKey(InputMap * self, Atom actionID, unsigned int keyCode, unsigned int charCode);
void InputMap_unbindKey(InputMap * self, Atom actionID, unsigned int keyCode);
bool InputMap_isKeyModifierBound(InputMap * self, Atom actionID, int modifierBit);
void InputMap_bindKeyModifier(InputMap * self, Atom actionID, int modifierBit);
void InputMap_unbindKeyModifier(InputMap * self, Atom actionID, int modifierBit);
bool InputMap_isButtonBound(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int buttonID);
void InputMap_bindButton(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int buttonID);
void InputMap_unbindButton(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int buttonID);
bool InputMap_isAxisBound(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int axisID);
void InputMap_bindAxis(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int axisID, float triggerThreshold, float releaseThreshold);
void InputMap_unbindAxis(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int axisID);

#ifdef __cplusplus
}
#endif
#endif
