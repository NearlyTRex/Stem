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

#include "inputcontroller/InputMap.h"

#define SUPERCLASS StemObject

InputMap * InputMap_create() {
	stemobject_create_implementation(InputMap, init)
}

bool InputMap_init(InputMap * self) {
	call_super(init, self);
	self->dispose = InputMap_dispose;
	
	self->keyboardBindingCount = 0;
	self->keyboardBindings = NULL;
	self->keyModifierBindingCount = 0;
	self->keyModifierBindings = NULL;
	self->gamepadMapCount = 0;
	self->gamepadMaps = NULL;
	return true;
}

void InputMap_dispose(InputMap * self) {
	unsigned int gamepadMapIndex;
	
	free(self->keyboardBindings);
	free(self->keyModifierBindings);
	for (gamepadMapIndex = 0; gamepadMapIndex < self->gamepadMapCount; gamepadMapIndex++) {
		free(self->gamepadMaps[gamepadMapIndex].buttonBindings);
		free(self->gamepadMaps[gamepadMapIndex].axisBindings);
	}
	free(self->gamepadMaps);
	call_super(dispose, self);
}

InputMap * InputMap_deserialize(compat_type(DeserializationContext *) deserializationContext) {
	stemobject_deserialize_implementation(InputMap, deserializationContext, loadSerializedData)
}

bool InputMap_loadSerializedData(InputMap * self, compat_type(DeserializationContext *) deserializationContext) {
	DeserializationContext * context = deserializationContext;
	uint16_t formatVersion;
	unsigned int keyboardBindingCount, bindingIndex;
	struct InputMap_keyboardBinding * keyboardBindings = NULL;
	const char * actionID;
	unsigned int gamepadMapCount, gamepadMapIndex;
	struct InputMap_gamepadMap * gamepadMaps;
	
	context->beginStructure(context, "input_map");
	formatVersion = context->readUInt16(context, "format_version");
	if (context->status == SERIALIZATION_ERROR_OK && formatVersion != INPUT_MAP_SERIALIZATION_FORMAT_VERSION) {
		return false;
	}
	keyboardBindingCount = context->beginArray(context, "keyboard_bindings");
	keyboardBindings = malloc(sizeof(struct InputMap_keyboardBinding) * keyboardBindingCount);
	for (bindingIndex = 0; bindingIndex < keyboardBindingCount; bindingIndex++) {
		context->beginStructure(context, NULL);
		keyboardBindings[bindingIndex].actionID = Atom_fromString(context->readString(context, "action"));
		keyboardBindings[bindingIndex].keyCode = context->readUInt32(context, "key_code");
		context->endStructure(context);
	}
	context->endArray(context);
	gamepadMapCount = context->beginArray(context, "gamepad_maps");
	gamepadMaps = calloc(sizeof(struct InputMap_gamepadMap), gamepadMapCount);
	for (gamepadMapIndex = 0; gamepadMapIndex < gamepadMapCount; gamepadMapIndex++) {
		context->beginStructure(context, NULL);
		gamepadMaps[gamepadMapIndex].vendorID = context->readInt32(context, "vendor_id");
		gamepadMaps[gamepadMapIndex].productID = context->readInt32(context, "product_id");
		gamepadMaps[gamepadMapIndex].buttonBindingCount = context->beginDictionary(context, "button_bindings");
		gamepadMaps[gamepadMapIndex].buttonBindings = malloc(sizeof(struct InputMap_gamepadButtonBinding) * gamepadMaps[gamepadMapIndex].buttonBindingCount);
		for (bindingIndex = 0; bindingIndex < gamepadMaps[gamepadMapIndex].buttonBindingCount; bindingIndex++) {
			actionID = context->readNextDictionaryKey(context);
			gamepadMaps[gamepadMapIndex].buttonBindings[bindingIndex].actionID = Atom_fromString(actionID);
			gamepadMaps[gamepadMapIndex].buttonBindings[bindingIndex].buttonID = context->readUInt32(context, actionID);
		}
		context->endDictionary(context);
		gamepadMaps[gamepadMapIndex].axisBindingCount = context->beginArray(context, "axis_bindings");
		gamepadMaps[gamepadMapIndex].axisBindings = malloc(sizeof(struct InputMap_gamepadAxisBinding) * gamepadMaps[gamepadMapIndex].axisBindingCount);
		for (bindingIndex = 0; bindingIndex < gamepadMaps[gamepadMapIndex].axisBindingCount; bindingIndex++) {
			context->beginStructure(context, NULL);
			gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].actionID = Atom_fromString(context->readString(context, "action"));
			gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].axisID = context->readUInt32(context, "axis_id");
			gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].triggerThreshold = context->readFloat(context, "trigger_threshold");
			gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].releaseThreshold = context->readFloat(context, "release_threshold");
			context->endStructure(context);
		}
		context->endArray(context);
		context->endStructure(context);
	}
	context->endArray(context);
	context->endStructure(context);
	
	if (context->status != SERIALIZATION_ERROR_OK) {
		free(keyboardBindings);
		for (gamepadMapIndex = 0; gamepadMapIndex < gamepadMapCount; gamepadMapIndex++) {
			free(gamepadMaps[gamepadMapIndex].buttonBindings);
			free(gamepadMaps[gamepadMapIndex].axisBindings);
		}
		free(gamepadMaps);
		return false;
	}
	
	InputMap_init(self);
	self->keyboardBindingCount = keyboardBindingCount;
	self->keyboardBindings = keyboardBindings;
	self->gamepadMapCount = gamepadMapCount;
	self->gamepadMaps = gamepadMaps;
	
	return true;
}

void InputMap_serialize(InputMap * self, compat_type(SerializationContext *) serializationContext) {
	SerializationContext * context = serializationContext;
	unsigned int bindingIndex;
	unsigned int gamepadMapIndex;
	
	context->beginStructure(context, "input_map");
	context->writeUInt16(context, "format_version", INPUT_MAP_SERIALIZATION_FORMAT_VERSION);
	context->beginArray(context, "keyboard_bindings");
	for (bindingIndex = 0; bindingIndex < self->keyboardBindingCount; bindingIndex++) {
		context->beginStructure(context, NULL);
		context->writeString(context, "action", self->keyboardBindings[bindingIndex].actionID);
		context->writeUInt32(context, "key_code", self->keyboardBindings[bindingIndex].keyCode);
		context->endStructure(context);
	}
	context->endArray(context);
	context->beginArray(context, "gamepad_maps");
	for (gamepadMapIndex = 0; gamepadMapIndex < self->gamepadMapCount; gamepadMapIndex++) {
		context->beginStructure(context, NULL);
		context->writeInt32(context, "vendor_id", self->gamepadMaps[gamepadMapIndex].vendorID);
		context->writeInt32(context, "product_id", self->gamepadMaps[gamepadMapIndex].productID);
		context->beginDictionary(context, "button_bindings");
		for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadMapIndex].buttonBindingCount; bindingIndex++) {
			context->writeUInt32(context, self->gamepadMaps[gamepadMapIndex].buttonBindings[bindingIndex].actionID, self->gamepadMaps[gamepadMapIndex].buttonBindings[bindingIndex].buttonID);
		}
		context->endDictionary(context);
		context->beginArray(context, "axis_bindings");
		for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadMapIndex].axisBindingCount; bindingIndex++) {
			context->beginStructure(context, NULL);
			context->writeString(context, "action", self->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].actionID);
			context->writeUInt32(context, "axis_id", self->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].axisID);
			context->writeFloat(context, "trigger_threshold", self->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].triggerThreshold);
			context->writeFloat(context, "release_threshold", self->gamepadMaps[gamepadMapIndex].axisBindings[bindingIndex].releaseThreshold);
			context->endStructure(context);
		}
		context->endArray(context);
		context->endStructure(context);
	}
	context->endArray(context);
	context->endStructure(context);
}

bool InputMap_isKeyBound(InputMap * self, Atom actionID, unsigned int keyCode) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyboardBindingCount; bindingIndex++) {
		if (self->keyboardBindings[bindingIndex].actionID == actionID && self->keyboardBindings[bindingIndex].keyCode == keyCode) {
			return true;
		}
	}
	return false;
}

bool InputMap_isKeyModifierBound(InputMap * self, Atom actionID, int modifierBit) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyModifierBindingCount; bindingIndex++) {
		if (self->keyModifierBindings[bindingIndex].actionID == actionID && self->keyModifierBindings[bindingIndex].modifierBit == modifierBit) {
			return true;
		}
	}
	return false;
}

bool InputMap_isButtonBound(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int buttonID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].actionID == actionID && self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].buttonID == buttonID) {
					return true;
				}
			}
		}
	}
	return false;
}

bool InputMap_isAxisBound(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int axisID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].actionID == actionID && self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].axisID == axisID) {
					return true;
				}
			}
		}
	}
	return false;
}

void InputMap_bindKey(InputMap * self, Atom actionID, unsigned int keyCode) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyboardBindingCount; bindingIndex++) {
		if (self->keyboardBindings[bindingIndex].actionID == actionID && self->keyboardBindings[bindingIndex].keyCode == keyCode) {
			return;
		}
	}
	self->keyboardBindings = realloc(self->keyboardBindings, sizeof(struct InputMap_keyboardBinding) * (self->keyboardBindingCount + 1));
	self->keyboardBindings[self->keyboardBindingCount].actionID = actionID;
	self->keyboardBindings[self->keyboardBindingCount].keyCode = keyCode;
	self->keyboardBindingCount++;
}

void InputMap_bindKeyModifier(InputMap * self, Atom actionID, int modifierBit) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyModifierBindingCount; bindingIndex++) {
		if (self->keyModifierBindings[bindingIndex].actionID == actionID && self->keyModifierBindings[bindingIndex].modifierBit == modifierBit) {
			return;
		}
	}
	self->keyModifierBindings = realloc(self->keyModifierBindings, sizeof(struct InputMap_keyModifierBinding) * (self->keyModifierBindingCount + 1));
	self->keyModifierBindings[self->keyModifierBindingCount].actionID = actionID;
	self->keyModifierBindings[self->keyModifierBindingCount].modifierBit = modifierBit;
	self->keyModifierBindingCount++;
}

void InputMap_bindButton(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int buttonID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].actionID == actionID && self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].buttonID == buttonID) {
					return;
				}
			}
			self->gamepadMaps[gamepadIndex].buttonBindings = realloc(self->gamepadMaps[gamepadIndex].buttonBindings, sizeof(struct InputMap_gamepadButtonBinding) * (self->gamepadMaps[gamepadIndex].buttonBindingCount + 1));
			self->gamepadMaps[gamepadIndex].buttonBindings[self->gamepadMaps[gamepadIndex].buttonBindingCount].actionID = actionID;
			self->gamepadMaps[gamepadIndex].buttonBindings[self->gamepadMaps[gamepadIndex].buttonBindingCount].buttonID = buttonID;
			self->gamepadMaps[gamepadIndex].buttonBindingCount++;
			
			return;
		}
	}
	self->gamepadMaps = realloc(self->gamepadMaps, sizeof(struct InputMap_gamepadMap) * (self->gamepadMapCount + 1));
	self->gamepadMaps[self->gamepadMapCount].vendorID = vendorID;
	self->gamepadMaps[self->gamepadMapCount].productID = productID;
	self->gamepadMaps[self->gamepadMapCount].buttonBindingCount = 1;
	self->gamepadMaps[self->gamepadMapCount].buttonBindings = malloc(sizeof(struct InputMap_gamepadButtonBinding));
	self->gamepadMaps[self->gamepadMapCount].buttonBindings[0].actionID = actionID;
	self->gamepadMaps[self->gamepadMapCount].buttonBindings[0].buttonID = buttonID;
	self->gamepadMaps[self->gamepadMapCount].axisBindingCount = 0;
	self->gamepadMaps[self->gamepadMapCount].axisBindings = NULL;
	self->gamepadMapCount++;
}

void InputMap_bindAxis(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int axisID, float triggerThreshold, float releaseThreshold) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].actionID == actionID && self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].axisID == axisID) {
					self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].triggerThreshold = triggerThreshold;
					self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].releaseThreshold = releaseThreshold;
					return;
				}
			}
			self->gamepadMaps[gamepadIndex].axisBindings = realloc(self->gamepadMaps[gamepadIndex].axisBindings, sizeof(struct InputMap_gamepadAxisBinding) * (self->gamepadMaps[gamepadIndex].axisBindingCount + 1));
			self->gamepadMaps[gamepadIndex].axisBindings[self->gamepadMaps[gamepadIndex].axisBindingCount].actionID = actionID;
			self->gamepadMaps[gamepadIndex].axisBindings[self->gamepadMaps[gamepadIndex].axisBindingCount].axisID = axisID;
			self->gamepadMaps[gamepadIndex].axisBindings[self->gamepadMaps[gamepadIndex].axisBindingCount].triggerThreshold = triggerThreshold;
			self->gamepadMaps[gamepadIndex].axisBindings[self->gamepadMaps[gamepadIndex].axisBindingCount].releaseThreshold = releaseThreshold;
			self->gamepadMaps[gamepadIndex].axisBindingCount++;
			
			return;
		}
	}
	self->gamepadMaps = realloc(self->gamepadMaps, sizeof(struct InputMap_gamepadMap) * (self->gamepadMapCount + 1));
	self->gamepadMaps[self->gamepadMapCount].vendorID = vendorID;
	self->gamepadMaps[self->gamepadMapCount].productID = productID;
	self->gamepadMaps[self->gamepadMapCount].buttonBindingCount = 0;
	self->gamepadMaps[self->gamepadMapCount].buttonBindings = NULL;
	self->gamepadMaps[self->gamepadMapCount].axisBindingCount = 1;
	self->gamepadMaps[self->gamepadMapCount].axisBindings = malloc(sizeof(struct InputMap_gamepadAxisBinding));
	self->gamepadMaps[self->gamepadMapCount].axisBindings[0].actionID = actionID;
	self->gamepadMaps[self->gamepadMapCount].axisBindings[0].axisID = axisID;
	self->gamepadMaps[self->gamepadMapCount].axisBindings[0].triggerThreshold = triggerThreshold;
	self->gamepadMaps[self->gamepadMapCount].axisBindings[0].releaseThreshold = releaseThreshold;
	self->gamepadMapCount++;
}

static void removeKeyBinding(InputMap * self, unsigned int bindingIndex) {
	self->keyboardBindingCount--;
	for (; bindingIndex < self->keyboardBindingCount; bindingIndex++) {
		self->keyboardBindings[bindingIndex] = self->keyboardBindings[bindingIndex + 1];
	}
}

static void removeKeyModifierBinding(InputMap * self, unsigned int bindingIndex) {
	self->keyModifierBindingCount--;
	for (; bindingIndex < self->keyModifierBindingCount; bindingIndex++) {
		self->keyModifierBindings[bindingIndex] = self->keyModifierBindings[bindingIndex + 1];
	}
}

static void removeButtonBinding(InputMap * self, unsigned int gamepadIndex, unsigned int bindingIndex) {
	self->gamepadMaps[gamepadIndex].buttonBindingCount--;
	for (; bindingIndex < self->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
		self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex] = self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex + 1];
	}
}

static void removeAxisBinding(InputMap * self, unsigned int gamepadIndex, unsigned int bindingIndex) {
	self->gamepadMaps[gamepadIndex].axisBindingCount--;
	for (; bindingIndex < self->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
		self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex] = self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex + 1];
	}
}

void InputMap_unbindKey(InputMap * self, Atom actionID, unsigned int keyCode) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyboardBindingCount; bindingIndex++) {
		if (self->keyboardBindings[bindingIndex].actionID == actionID && self->keyboardBindings[bindingIndex].keyCode == keyCode) {
			removeKeyBinding(self, bindingIndex);
			break;
		}
	}
}

void InputMap_unbindKeyModifier(InputMap * self, Atom actionID, int modifierBit) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyModifierBindingCount; bindingIndex++) {
		if (self->keyModifierBindings[bindingIndex].actionID == actionID && self->keyModifierBindings[bindingIndex].modifierBit == modifierBit) {
			removeKeyModifierBinding(self, bindingIndex);
			break;
		}
	}
}

void InputMap_unbindButton(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int buttonID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].actionID == actionID && self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].buttonID == buttonID) {
					removeButtonBinding(self, gamepadIndex, bindingIndex);
					return;
				}
			}
			return;
		}
	}
}

void InputMap_unbindAxis(InputMap * self, Atom actionID, int vendorID, int productID, unsigned int axisID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].actionID == actionID && self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].axisID == axisID) {
					removeAxisBinding(self, gamepadIndex, bindingIndex);
					return;
				}
			}
			return;
		}
	}
}

void InputMap_unbindAllKeysForAction(InputMap * self, Atom actionID) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyboardBindingCount; bindingIndex++) {
		if (self->keyboardBindings[bindingIndex].actionID == actionID) {
			removeKeyBinding(self, bindingIndex);
			bindingIndex--;
		}
	}
}

void InputMap_unbindAllKeyModifiersForAction(InputMap * self, Atom actionID) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyModifierBindingCount; bindingIndex++) {
		if (self->keyModifierBindings[bindingIndex].actionID == actionID) {
			removeKeyModifierBinding(self, bindingIndex);
			bindingIndex--;
		}
	}
}

void InputMap_unbindAllButtonsForAction(InputMap * self, Atom actionID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
			if (self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].actionID == actionID) {
				removeButtonBinding(self, gamepadIndex, bindingIndex);
				bindingIndex--;
			}
		}
	}
}

void InputMap_unbindAllAxesForAction(InputMap * self, Atom actionID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
			if (self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].actionID == actionID) {
				removeAxisBinding(self, gamepadIndex, bindingIndex);
				bindingIndex--;
			}
		}
	}
}

void InputMap_unbindAllButtonsOnDeviceForAction(InputMap * self, int vendorID, int productID, Atom actionID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].actionID == actionID) {
					removeButtonBinding(self, gamepadIndex, bindingIndex);
					bindingIndex--;
				}
			}
			return;
		}
	}
}

void InputMap_unbindAllAxesOnDeviceForAction(InputMap * self, int vendorID, int productID, Atom actionID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].actionID == actionID) {
					removeAxisBinding(self, gamepadIndex, bindingIndex);
					bindingIndex--;
				}
			}
			return;
		}
	}
}

void InputMap_unbindAllActionsForKey(InputMap * self, unsigned int keyCode) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyboardBindingCount; bindingIndex++) {
		if (self->keyboardBindings[bindingIndex].keyCode == keyCode) {
			removeKeyBinding(self, bindingIndex);
			bindingIndex--;
		}
	}
}

void InputMap_unbindAllActionsForKeyModifier(InputMap * self, int modifierBit) {
	unsigned int bindingIndex;
	
	for (bindingIndex = 0; bindingIndex < self->keyModifierBindingCount; bindingIndex++) {
		if (self->keyModifierBindings[bindingIndex].modifierBit == modifierBit) {
			removeKeyModifierBinding(self, bindingIndex);
			bindingIndex--;
		}
	}
}

void InputMap_unbindAllActionsForButton(InputMap * self, int vendorID, int productID, unsigned int buttonID) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].buttonID == buttonID) {
					removeButtonBinding(self, gamepadIndex, bindingIndex);
					bindingIndex--;
				}
			}
			return;
		}
	}
}

void InputMap_unbindAllActionsForAxis(InputMap * self, int vendorID, int productID, unsigned int axisID, bool positive) {
	unsigned int gamepadIndex, bindingIndex;
	
	for (gamepadIndex = 0; gamepadIndex < self->gamepadMapCount; gamepadIndex++) {
		if (self->gamepadMaps[gamepadIndex].vendorID == vendorID && self->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
				if (self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].axisID == axisID && (self->gamepadMaps[gamepadIndex].axisBindings[bindingIndex].triggerThreshold > 0.0f) == positive) {
					removeAxisBinding(self, gamepadIndex, bindingIndex);
					bindingIndex--;
				}
			}
			return;
		}
	}
}
