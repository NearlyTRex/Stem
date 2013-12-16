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

#include "inputcontroller/InputController.h"
#include <limits.h>
#include <math.h>

#define SUPERCLASS StemObject

InputController * InputController_create(InputMap * inputMap, ...) {
	InputController * self;
	va_list args;
	
	va_start(args, inputMap);
	self = InputController_vcreate(inputMap, args);
	va_end(args);
	return self;
}

InputController * InputController_vcreate(InputMap * inputMap, va_list args) {
	stemobject_create_implementation(InputController, vinit, inputMap, args)
}

void InputController_init(InputController * self, InputMap * inputMap, ...) {
	va_list args;
	
	va_start(args, inputMap);
	InputController_vinit(self, inputMap, args);
	va_end(args);
}

void InputController_vinit(InputController * self, InputMap * inputMap, va_list args) {
	va_list argsCopy;
	
	call_super(init, self);
	self->dispose = InputController_dispose;
	self->keyDown = InputController_keyDown;
	self->keyUp = InputController_keyUp;
	self->keyModifiersChanged = InputController_keyModifiersChanged;
	self->gamepadButtonDown = InputController_gamepadButtonDown;
	self->gamepadButtonUp = InputController_gamepadButtonUp;
	self->gamepadAxisMoved = InputController_gamepadAxisMoved;
	self->triggerAction = InputController_triggerAction;
	self->releaseAction = InputController_releaseAction;
	
	self->eventDispatcher = EventDispatcher_create(self);
	self->inputMap = inputMap;
	self->lastModifiers = 0x0;
	self->actionCount = 0;
	self->actions = NULL;
	
	va_copy(argsCopy, args);
	while (va_arg(argsCopy, const char *) != NULL) {
		self->actionCount++;
	}
	va_end(argsCopy);
	
	if (self->actionCount > 0) {
		unsigned int actionIndex;
		
		self->actions = malloc(sizeof(struct InputController_action) * self->actionCount);
		for (actionIndex = 0; actionIndex < self->actionCount; actionIndex++) {
			self->actions[actionIndex].actionID = Atom_fromString(va_arg(args, const char *));
			self->actions[actionIndex].triggered = false;
		}
	}
}

void InputController_dispose(InputController * self) {
	self->eventDispatcher->dispose(self->eventDispatcher);
	free(self->actions);
	call_super(dispose, self);
}

bool InputController_keyDown(InputController * self, unsigned int keyCode) {
	unsigned int bindingIndex;
	bool handled = false;
	
	if (self->inputMap == NULL) {
		return false;
	}
	for (bindingIndex = 0; bindingIndex < self->inputMap->keyboardBindingCount; bindingIndex++) {
		if (self->inputMap->keyboardBindings[bindingIndex].keyCode == keyCode) {
			handled = self->triggerAction(self, self->inputMap->keyboardBindings[bindingIndex].actionID) || handled;
		}
	}
	return handled;
}

bool InputController_keyUp(InputController * self, unsigned int keyCode) {
	unsigned int bindingIndex;
	bool handled = false;
	
	if (self->inputMap == NULL) {
		return false;
	}
	for (bindingIndex = 0; bindingIndex < self->inputMap->keyboardBindingCount; bindingIndex++) {
		if (self->inputMap->keyboardBindings[bindingIndex].keyCode == keyCode) {
			handled = self->releaseAction(self, self->inputMap->keyboardBindings[bindingIndex].actionID) || handled;
		}
	}
	return handled;
}

bool InputController_keyModifiersChanged(InputController * self, unsigned int modifiers) {
	unsigned int bindingIndex;
	bool handled = false;
	unsigned int lastModifier, modifier;
	
	if (self->inputMap == NULL) {
		self->lastModifiers = modifiers;
		return false;
	}
	for (bindingIndex = 0; bindingIndex < self->inputMap->keyModifierBindingCount; bindingIndex++) {
		lastModifier = self->inputMap->keyModifierBindings[bindingIndex].modifierBit & self->lastModifiers;
		modifier = self->inputMap->keyModifierBindings[bindingIndex].modifierBit & modifiers;
		if (lastModifier && !modifier) {
			handled = self->releaseAction(self, self->inputMap->keyModifierBindings[bindingIndex].actionID) || handled;
		} else if (modifier && !lastModifier) {
			handled = self->triggerAction(self, self->inputMap->keyModifierBindings[bindingIndex].actionID) || handled;
		}
	}
	self->lastModifiers = modifiers;
	return false;
}

bool InputController_gamepadButtonDown(InputController * self, int vendorID, int productID, unsigned int buttonID) {
	unsigned int gamepadIndex, bindingIndex;
	bool handled = false;
	
	if (self->inputMap == NULL) {
		return false;
	}
	for (gamepadIndex = 0; gamepadIndex < self->inputMap->gamepadMapCount; gamepadIndex++) {
		if (self->inputMap->gamepadMaps[gamepadIndex].vendorID == vendorID && self->inputMap->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->inputMap->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
				if (self->inputMap->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].buttonID == buttonID) {
					handled = self->triggerAction(self, self->inputMap->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex].actionID) || handled;
				}
			}
			break;
		}
	}
	return handled;
}

bool InputController_gamepadButtonUp(InputController * self, int vendorID, int productID, unsigned int buttonID) {
	unsigned int gamepadIndex, bindingIndex;
	bool handled = false;
	struct InputMap_gamepadButtonBinding binding;
	
	if (self->inputMap == NULL) {
		return false;
	}
	for (gamepadIndex = 0; gamepadIndex < self->inputMap->gamepadMapCount; gamepadIndex++) {
		if (self->inputMap->gamepadMaps[gamepadIndex].vendorID == vendorID && self->inputMap->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->inputMap->gamepadMaps[gamepadIndex].buttonBindingCount; bindingIndex++) {
				binding = self->inputMap->gamepadMaps[gamepadIndex].buttonBindings[bindingIndex];
				if (binding.buttonID == buttonID) {
					handled = self->releaseAction(self, binding.actionID) || handled;
				}
			}
			break;
		}
	}
	return handled;
}

bool InputController_gamepadAxisMoved(InputController * self, int vendorID, int productID, unsigned int axisID, float value, float lastValue) {
	unsigned int gamepadIndex, bindingIndex;
	struct InputMap_gamepadAxisBinding binding;
	bool handled = false;
	int direction;
	
	if (self->inputMap == NULL) {
		return false;
	}
	for (gamepadIndex = 0; gamepadIndex < self->inputMap->gamepadMapCount; gamepadIndex++) {
		if (self->inputMap->gamepadMaps[gamepadIndex].vendorID == vendorID && self->inputMap->gamepadMaps[gamepadIndex].productID == productID) {
			for (bindingIndex = 0; bindingIndex < self->inputMap->gamepadMaps[gamepadIndex].axisBindingCount; bindingIndex++) {
				binding = self->inputMap->gamepadMaps[gamepadIndex].axisBindings[bindingIndex];
				if (binding.axisID == axisID) {
					if (binding.triggerThreshold > binding.releaseThreshold) {
						direction = 1;
					} else if (binding.triggerThreshold < binding.releaseThreshold) {
						direction = -1;
					} else if (binding.triggerThreshold > 0) {
						direction = 1;
					} else {
						direction = -1;
					}
					if (lastValue * direction <  binding.triggerThreshold * direction &&
					    value     * direction >= binding.triggerThreshold * direction) {
						handled = self->triggerAction(self, binding.actionID) || handled;
						
					} else if (lastValue * direction >= binding.releaseThreshold * direction &&
					           value     * direction <  binding.releaseThreshold * direction) {
						handled = self->releaseAction(self, binding.actionID) || handled;
					}
				}
			}
			break;
		}
	}
	return handled;
}

bool InputController_triggerAction(InputController * self, Atom actionID) {
	unsigned int actionIndex;
	
	for (actionIndex = 0; actionIndex < self->actionCount; actionIndex++) {
		if (actionID == self->actions[actionIndex].actionID) {
			if (!self->actions[actionIndex].triggered) {
				self->actions[actionIndex].triggered = true;
				self->eventDispatcher->dispatchEvent(self->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_DOWN), (void *) self->actions[actionIndex].actionID);
			}
			return true;
		}
	}
	return false;
}

bool InputController_releaseAction(InputController * self, Atom actionID) {
	unsigned int actionIndex;
	
	for (actionIndex = 0; actionIndex < self->actionCount; actionIndex++) {
		if (actionID == self->actions[actionIndex].actionID) {
			if (self->actions[actionIndex].triggered) {
				self->actions[actionIndex].triggered = false;
				self->eventDispatcher->dispatchEvent(self->eventDispatcher, ATOM(INPUT_CONTROLLER_EVENT_ACTION_UP), (void *) self->actions[actionIndex].actionID);
			}
			return true;
		}
	}
	return false;
}
