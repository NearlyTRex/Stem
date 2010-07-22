/*
  Copyright (c) 2010 Alex Diener
  
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

#define StateController_structContents \
	StemObject_structContents \
	\
	EventDispatcher * eventDispatcher; \
	State * currentState; \
	size_t validStateCount; \
	State ** validStates; \
	size_t transitionCount; \
	struct StateController_transition * transitions; \
	\
	void (* addState)(void * self, State * state); \
	void (* setState)(void * self, State * state); \
	void (* addTransition)(void * self, State * fromState, State * toState, const char * transitionName); \
	void (* transition)(void * self, const char * transitionName);

struct StateController {
	StateController_structContents
};

StateController * StateController_create();
void StateController_init(StateController * self);
void StateController_dispose(void * selfPtr);

void StateController_addState(void * selfPtr, State * state);
void StateController_setState(void * selfPtr, State * state);
void StateController_addTransition(void * selfPtr, State * fromState, State * toState, const char * transitionName);
void StateController_transition(void * selfPtr, const char * transitionName);

#endif
