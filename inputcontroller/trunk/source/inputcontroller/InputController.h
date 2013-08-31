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

#ifndef __InputController_H__
#define __InputController_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct InputController InputController;

#include "inputcontroller/InputMap.h"
#include "gamepad/Gamepad.h"
#include "stemobject/StemObject.h"
#include "utilities/EventDispatcher.h"
#include <stdarg.h>

// eventData -> action
#define INPUT_CONTROLLER_EVENT_ACTION_DOWN "INPUT_CONTROLLER_EVENT_ACTION_DOWN"
#define INPUT_CONTROLLER_EVENT_ACTION_UP "INPUT_CONTROLLER_EVENT_ACTION_UP"

struct InputController_action {
	Atom actionID;
	bool triggered;
};

#define InputController_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	EventDispatcher * eventDispatcher; \
	InputMap * inputMap; \
	unsigned int actionCount; \
	struct InputController_action * actions; \
	\
	bool (* keyDown)(self_type * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers); \
	bool (* keyUp)(self_type * self, unsigned int keyCode, unsigned int modifiers); \
	bool (* keyModifiersChanged)(self_type * self, unsigned int modifiers); \
	bool (* gamepadButtonDown)(self_type * self, struct Gamepad_device * device, unsigned int buttonID, double timestamp); \
	bool (* gamepadButtonUp)(self_type * self, struct Gamepad_device * device, unsigned int buttonID, double timestamp); \
	bool (* gamepadAxisMoved)(self_type * self, struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp); \
	bool (* protected_ivar(triggerAction))(self_type * self, Atom actionID); \
	bool (* protected_ivar(releaseAction))(self_type * self, Atom actionID);

stemobject_struct_definition(InputController)

// Additional arguments: List of all valid action IDs
InputController * InputController_create(InputMap * inputMap, ...) __attribute__((sentinel));
InputController * InputController_vcreate(InputMap * inputMap, va_list args);
void InputController_init(InputController * self, InputMap * inputMap, ...) __attribute__((sentinel));
void InputController_vinit(InputController * self, InputMap * inputMap, va_list args);
void InputController_dispose(InputController * self);

bool InputController_keyDown(InputController * self, unsigned int charCode, unsigned int keyCode, unsigned int modifiers);
bool InputController_keyUp(InputController * self, unsigned int keyCode, unsigned int modifiers);
bool InputController_keyModifiersChanged(InputController * self, unsigned int modifiers);
bool InputController_gamepadButtonDown(InputController * self, struct Gamepad_device * device, unsigned int buttonID, double timestamp);
bool InputController_gamepadButtonUp(InputController * self, struct Gamepad_device * device, unsigned int buttonID, double timestamp);
bool InputController_gamepadAxisMoved(InputController * self, struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp);
bool InputController_triggerAction(InputController * self, Atom actionID);
bool InputController_releaseAction(InputController * self, Atom actionID);

#ifdef __cplusplus
}
#endif
#endif
