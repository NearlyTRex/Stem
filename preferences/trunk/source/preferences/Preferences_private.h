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

#ifndef __PREFERENCES_PRIVATE_H__
#define __PREFERENCES_PRIVATE_H__

#include "preferences/Preferences.h"

void Preferences_setIntegerPrivate(compat_type(Preferences *) selfPtr, const char * name, int value);
void Preferences_setFloatPrivate(compat_type(Preferences *) selfPtr, const char * name, float value);
void Preferences_setBooleanPrivate(compat_type(Preferences *) selfPtr, const char * name, bool value);
void Preferences_setStringPrivate(compat_type(Preferences *) selfPtr, const char * name, const char * value);
void Preferences_setDataPrivate(compat_type(Preferences *) selfPtr, const char * name, const void * value, size_t length);

void Preferences_loadPrivate(compat_type(Preferences *) selfPtr);
void Preferences_savePrivate(compat_type(Preferences *) selfPtr);

#endif
