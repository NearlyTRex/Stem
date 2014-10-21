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

#ifndef __WGLShellCallbacks_H__
#define __WGLShellCallbacks_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

extern bool (* drawCallback)(void) __asm("drawCallback$WGLShell");
extern void (* resizeCallback)(unsigned int newWidth, unsigned int newHeight) __asm("resizeCallback$WGLShell");
extern void (* keyDownCallback)(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) __asm("keyDownCallback$WGLShell");
extern void (* keyUpCallback)(unsigned int keyCode, unsigned int modifierFlags) __asm("keyUpCallback$WGLShell");
extern void (* keyModifiersChangedCallback)(unsigned int modifierFlags) __asm("keyModifiersChangedCallback$WGLShell");
extern void (* mouseDownCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseDownCallback$WGLShell");
extern void (* mouseUpCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseUpCallback$WGLShell");
extern void (* mouseMovedCallback)(float x, float y) __asm("mouseMovedCallback$WGLShell");
extern void (* mouseDraggedCallback)(unsigned int buttonMask, float x, float y) __asm("mouseDraggedCallback$WGLShell");
extern void (* scrollWheelCallback)(int deltaX, int deltaY) __asm("scrollWheelCallback$WGLShell");
extern void (* backgroundedCallback)(void) __asm("backgroundedCallback$WGLShell");
extern void (* foregroundedCallback)(void) __asm("foregroundedCallback$WGLShell");

#ifdef __cplusplus
}
#endif
#endif
