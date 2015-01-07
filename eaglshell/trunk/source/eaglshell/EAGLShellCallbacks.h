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

#ifndef __EAGLShellCallbacks_H__
#define __EAGLShellCallbacks_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

extern bool (* drawCallback)(void) __asm("drawCallback$EAGLShell");
extern void (* resizeCallback)(unsigned int newWidth, unsigned int newHeight) __asm("resizeCallback$EAGLShell");
extern void (* keyDownCallback)(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) __asm("keyDownCallback$EAGLShell");
extern void (* keyUpCallback)(unsigned int keyCode, unsigned int modifierFlags) __asm("keyUpCallback$EAGLShell");
extern void (* keyModifiersChangedCallback)(unsigned int modifierFlags) __asm("keyModifiersChangedCallback$EAGLShell");
extern void (* mouseDownCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseDownCallback$EAGLShell");
extern void (* mouseUpCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseUpCallback$EAGLShell");
extern void (* mouseMovedCallback)(float x, float y) __asm("mouseMovedCallback$EAGLShell");
extern void (* mouseDraggedCallback)(unsigned int buttonMask, float x, float y) __asm("mouseDraggedCallback$EAGLShell");
extern void (* scrollWheelCallback)(int deltaX, int deltaY) __asm("scrollWheelCallback$EAGLShell");
extern void (* backgroundedCallback)(void) __asm("backgroundedCallback$EAGLShell");
extern void (* foregroundedCallback)(void) __asm("foregroundedCallback$EAGLShell");
extern void (* openURLCallback)(const char * url) __asm("openURLCallback$EAGLShell");
extern void (* touchesCancelledCallback)(unsigned int buttonMask) __asm("touchesCancelledCallback$EAGLShell");
extern void (* accelerometerCallback)(double x, double y, double z) __asm("accelerometerCallback$EAGLShell");

#ifdef __cplusplus
}
#endif
#endif
