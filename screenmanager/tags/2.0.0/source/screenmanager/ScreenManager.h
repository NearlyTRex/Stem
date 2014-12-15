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

#ifndef __ScreenManager_H__
#define __ScreenManager_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ScreenManager ScreenManager;

#include "stemobject/StemObject.h"
#include "screenmanager/Screen.h"
#include "utilities/EventDispatcher.h"
#include <stdlib.h>

struct ScreenManager_transition {
	Screen * fromScreen;
	Screen * toScreen;
	char * name;
};

#define ScreenManager_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	EventDispatcher * eventDispatcher; \
	Screen * currentScreen; \
	size_t validScreenCount; \
	Screen ** validScreens; \
	size_t transitionCount; \
	struct ScreenManager_transition * transitions;

stemobject_struct_definition(ScreenManager)

ScreenManager * ScreenManager_create();
bool ScreenManager_init(ScreenManager * self);
void ScreenManager_dispose(ScreenManager * self);

void ScreenManager_addScreen(ScreenManager * self, compat_type(Screen *) screen);
void ScreenManager_setScreen(ScreenManager * self, compat_type(Screen *) screen);
void ScreenManager_addTransition(ScreenManager * self, compat_type(Screen *) fromScreen, compat_type(Screen *) toScreen, const char * transitionName);
void ScreenManager_transition(ScreenManager * self, const char * transitionName);

#ifdef __cplusplus
}
#endif
#endif
