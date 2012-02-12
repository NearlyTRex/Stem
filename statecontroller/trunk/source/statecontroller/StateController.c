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

#include "statecontroller/StateController.h"
#include <string.h>

#define SUPERCLASS StemObject

StateController * StateController_create() {
	stemobject_create_implementation(StateController, init)
}

void StateController_init(StateController * self) {
	call_super(init, self);
	self->dispose = StateController_dispose;
	self->addState = StateController_addState;
	self->setState = StateController_setState;
	self->addTransition = StateController_addTransition;
	self->transition = StateController_transition;
	
	self->eventDispatcher = EventDispatcher_create(self);
	self->currentState = NULL;
	self->validStateCount = 0;
	self->validStates = NULL;
	self->transitionCount = 0;
	self->transitions = NULL;
}

void StateController_dispose(StateController * self) {
	size_t transitionIndex;
	
	self->eventDispatcher->dispose(self->eventDispatcher);
	free(self->validStates);
	for (transitionIndex = 0; transitionIndex < self->transitionCount; transitionIndex++) {
		free(self->transitions[transitionIndex].name);
	}
	free(self->transitions);
	
	call_super(dispose, self);
}

void StateController_addState(StateController * self, compat_type(State *) state) {
	size_t stateIndex;
	
	for (stateIndex = 0; stateIndex < self->validStateCount; stateIndex++) {
		if (self->validStates[stateIndex] == state) {
			break;
		}
	}
	if (stateIndex == self->validStateCount) {
		self->validStates = realloc(self->validStates, sizeof(State *) * (self->validStateCount + 1));
		self->validStates[self->validStateCount++] = state;
		((State *) state)->stateController = self;
	}
}

void StateController_setState(StateController * self, compat_type(State *) state) {
	size_t stateIndex;
	
	for (stateIndex = 0; stateIndex < self->validStateCount; stateIndex++) {
		if (self->validStates[stateIndex] == state) {
			if (self->currentState != NULL) {
				self->currentState->deactivate(self->currentState);
			}
			self->currentState = state;
			self->currentState->activate(self->currentState);
			break;
		}
	}
}

void StateController_addTransition(StateController * self, compat_type(State *) fromState, compat_type(State *) toState, const char * transitionName) {
	size_t stateIndex;
	bool fromStateValid = false, toStateValid = false;
	
	for (stateIndex = 0; stateIndex < self->validStateCount; stateIndex++) {
		if (self->validStates[stateIndex] == toState) {
			toStateValid = true;
		} else if (self->validStates[stateIndex] == fromState) {
			fromStateValid = true;
		}
		if (fromStateValid && toStateValid) {
			size_t transitionIndex;
			
			for (transitionIndex = 0; transitionIndex < self->transitionCount; transitionIndex++) {
				if (self->transitions[transitionIndex].fromState == fromState && !strcmp(transitionName, self->transitions[transitionIndex].name)) {
					return;
				}
			}
			self->transitions = realloc(self->transitions, sizeof(struct StateController_transition) * (self->transitionCount + 1));
			self->transitions[self->transitionCount].fromState = fromState;
			self->transitions[self->transitionCount].toState = toState;
			self->transitions[self->transitionCount].name = malloc(strlen(transitionName) + 1);
			strcpy(self->transitions[self->transitionCount].name, transitionName);
			self->transitionCount++;
			break;
		}
	}
}

void StateController_transition(StateController * self, const char * transitionName) {
	size_t transitionIndex;
	
	for (transitionIndex = 0; transitionIndex < self->transitionCount; transitionIndex++) {
		if (self->currentState == self->transitions[transitionIndex].fromState && !strcmp(transitionName, self->transitions[transitionIndex].name)) {
			self->currentState->deactivate(self->currentState);
			self->currentState = self->transitions[transitionIndex].toState;
			self->currentState->activate(self->currentState);
			break;
		}
	}
}
