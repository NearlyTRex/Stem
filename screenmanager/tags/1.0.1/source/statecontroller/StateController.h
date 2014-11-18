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

#ifndef __STATE_CONTROLLER_H__
#define __STATE_CONTROLLER_H__

typedef struct StateController StateController;

#include "stemobject/StemObject.h"
#include "statecontroller/State.h"
#include "utilities/EventDispatcher.h"
#include <stdlib.h>

struct StateController_transition {
	State * fromState;
	State * toState;
	char * name;
};

#define StateController_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	EventDispatcher * eventDispatcher; \
	State * currentState; \
	size_t validStateCount; \
	State ** validStates; \
	size_t transitionCount; \
	struct StateController_transition * transitions; \
	\
	void (* addState)(self_type * self, compat_type(State *) state); \
	void (* setState)(self_type * self, compat_type(State *) state); \
	void (* addTransition)(self_type * self, compat_type(State *) fromState, compat_type(State *) toState, const char * transitionName); \
	void (* transition)(self_type * self, const char * transitionName);

stemobject_struct_definition(StateController)

StateController * StateController_create();
void StateController_init(StateController * self);
void StateController_dispose(StateController * self);

void StateController_addState(StateController * self, compat_type(State *) state);
void StateController_setState(StateController * self, compat_type(State *) state);
void StateController_addTransition(StateController * self, compat_type(State *) fromState, compat_type(State *) toState, const char * transitionName);
void StateController_transition(StateController * self, const char * transitionName);

#endif
