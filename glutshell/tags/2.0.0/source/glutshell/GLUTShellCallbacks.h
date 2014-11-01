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

#ifndef __GLUTShellCallbacks_H__
#define __GLUTShellCallbacks_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

extern bool (* drawCallback)(void) __asm("drawCallback$GLUTShell");
extern void (* resizeCallback)(unsigned int newWidth, unsigned int newHeight) __asm("resizeCallback$GLUTShell");
extern void (* keyDownCallback)(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) __asm("keyDownCallback$GLUTShell");
extern void (* keyUpCallback)(unsigned int keyCode, unsigned int modifierFlags) __asm("keyUpCallback$GLUTShell");
extern void (* keyModifiersChangedCallback)(unsigned int modifierFlags) __asm("keyModifiersChangedCallback$GLUTShell");
extern void (* mouseDownCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseDownCallback$GLUTShell");
extern void (* mouseUpCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseUpCallback$GLUTShell");
extern void (* mouseMovedCallback)(float x, float y) __asm("mouseMovedCallback$GLUTShell");
extern void (* mouseDraggedCallback)(unsigned int buttonMask, float x, float y) __asm("mouseDraggedCallback$GLUTShell");
extern void (* scrollWheelCallback)(int deltaX, int deltaY) __asm("scrollWheelCallback$GLUTShell");
extern void (* backgroundedCallback)(void) __asm("backgroundedCallback$GLUTShell");
extern void (* foregroundedCallback)(void) __asm("foregroundedCallback$GLUTShell");

#ifdef __cplusplus
}
#endif
#endif
