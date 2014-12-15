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

#include "screenmanager/ScreenManager.h"
#include <string.h>
#include <stdio.h>

#define SUPERCLASS StemObject

ScreenManager * ScreenManager_create() {
	stemobject_create_implementation(ScreenManager, init)
}

bool ScreenManager_init(ScreenManager * self) {
	call_super(init, self);
	self->dispose = ScreenManager_dispose;
	
	self->eventDispatcher = EventDispatcher_create();
	self->currentScreen = NULL;
	self->validScreenCount = 0;
	self->validScreens = NULL;
	self->transitionCount = 0;
	self->transitions = NULL;
	return true;
}

void ScreenManager_dispose(ScreenManager * self) {
	size_t transitionIndex;
	
	self->eventDispatcher->dispose(self->eventDispatcher);
	free(self->validScreens);
	for (transitionIndex = 0; transitionIndex < self->transitionCount; transitionIndex++) {
		free(self->transitions[transitionIndex].name);
	}
	free(self->transitions);
	
	call_super(dispose, self);
}

void ScreenManager_addScreen(ScreenManager * self, compat_type(Screen *) screen) {
	size_t screenIndex;
	
	for (screenIndex = 0; screenIndex < self->validScreenCount; screenIndex++) {
		if (self->validScreens[screenIndex] == screen) {
			break;
		}
	}
	if (screenIndex == self->validScreenCount) {
		self->validScreens = realloc(self->validScreens, sizeof(Screen *) * (self->validScreenCount + 1));
		self->validScreens[self->validScreenCount++] = screen;
		((Screen *) screen)->screenManager = self;
	}
}

void ScreenManager_setScreen(ScreenManager * self, compat_type(Screen *) screen) {
	size_t screenIndex;
	
	for (screenIndex = 0; screenIndex < self->validScreenCount; screenIndex++) {
		if (self->validScreens[screenIndex] == screen) {
			if (self->currentScreen != NULL) {
				self->currentScreen->deactivate(self->currentScreen);
			}
			self->currentScreen = screen;
			self->currentScreen->activate(self->currentScreen);
			return;
		}
	}
#ifdef DEBUG
	fprintf(stderr, "Warning: ScreenManager_setScreen called with object %p, which has not been added with ScreenManager_addScreen\n", screen);
#endif
}

void ScreenManager_addTransition(ScreenManager * self, compat_type(Screen *) fromScreen, compat_type(Screen *) toScreen, const char * transitionName) {
	size_t screenIndex;
	bool fromScreenValid = false, toScreenValid = false;
	
	for (screenIndex = 0; screenIndex < self->validScreenCount; screenIndex++) {
		if (self->validScreens[screenIndex] == toScreen) {
			toScreenValid = true;
		} else if (self->validScreens[screenIndex] == fromScreen) {
			fromScreenValid = true;
		}
		if (fromScreenValid && toScreenValid) {
			size_t transitionIndex;
			
			for (transitionIndex = 0; transitionIndex < self->transitionCount; transitionIndex++) {
				if (self->transitions[transitionIndex].fromScreen == fromScreen && !strcmp(transitionName, self->transitions[transitionIndex].name)) {
					return;
				}
			}
			self->transitions = realloc(self->transitions, sizeof(struct ScreenManager_transition) * (self->transitionCount + 1));
			self->transitions[self->transitionCount].fromScreen = fromScreen;
			self->transitions[self->transitionCount].toScreen = toScreen;
			self->transitions[self->transitionCount].name = malloc(strlen(transitionName) + 1);
			strcpy(self->transitions[self->transitionCount].name, transitionName);
			self->transitionCount++;
			return;
		}
	}
#ifdef DEBUG
	fprintf(stderr, "Warning: ScreenManager_addTransition called with objects %p and %p, at least one of which has not been added with ScreenManager_addScreen\n", fromScreen, toScreen);
#endif
}

void ScreenManager_transition(ScreenManager * self, const char * transitionName) {
	size_t transitionIndex;
	
	for (transitionIndex = 0; transitionIndex < self->transitionCount; transitionIndex++) {
		if (self->currentScreen == self->transitions[transitionIndex].fromScreen && !strcmp(transitionName, self->transitions[transitionIndex].name)) {
			self->currentScreen->deactivate(self->currentScreen);
			self->currentScreen = self->transitions[transitionIndex].toScreen;
			self->currentScreen->activate(self->currentScreen);
			return;
		}
	}
#ifdef DEBUG
	fprintf(stderr, "Warning: ScreenManager_transition called with transition name \"%s\", which is not valid for screen %p\n", transitionName, self->currentScreen);
#endif
}
