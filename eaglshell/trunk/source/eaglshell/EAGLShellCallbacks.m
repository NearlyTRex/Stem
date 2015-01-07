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

#include "eaglshell/EAGLShellCallbacks.h"

bool (* drawCallback)(void) __asm("drawCallback$EAGLShell");
void (* resizeCallback)(unsigned int newWidth, unsigned int newHeight) __asm("resizeCallback$EAGLShell");
void (* keyDownCallback)(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags) __asm("keyDownCallback$EAGLShell");
void (* keyUpCallback)(unsigned int keyCode, unsigned int modifierFlags) __asm("keyUpCallback$EAGLShell");
void (* keyModifiersChangedCallback)(unsigned int modifierFlags) __asm("keyModifiersChangedCallback$EAGLShell");
void (* mouseDownCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseDownCallback$EAGLShell");
void (* mouseUpCallback)(unsigned int buttonNumber, float x, float y) __asm("mouseUpCallback$EAGLShell");
void (* mouseMovedCallback)(float x, float y) __asm("mouseMovedCallback$EAGLShell");
void (* mouseDraggedCallback)(unsigned int buttonMask, float x, float y) __asm("mouseDraggedCallback$EAGLShell");
void (* scrollWheelCallback)(int deltaX, int deltaY) __asm("scrollWheelCallback$EAGLShell");
void (* backgroundedCallback)(void) __asm("backgroundedCallback$EAGLShell");
void (* foregroundedCallback)(void) __asm("foregroundedCallback$EAGLShell");
void (* openURLCallback)(const char * url) __asm("openURLCallback$EAGLShell");
void (* touchesCancelledCallback)(unsigned int buttonMask) __asm("touchesCancelledCallback$EAGLShell");
void (* accelerometerCallback)(double x, double y, double z) __asm("accelerometerCallback$EAGLShell");

void Shell_drawFunc(bool (* callback)(void)) {
	drawCallback = callback;
}

void Shell_resizeFunc(void (* callback)(unsigned int newWidth, unsigned int newHeight)) {
	resizeCallback = callback;
}

void Shell_keyDownFunc(void (* callback)(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags)) {
	keyDownCallback = callback;
}

void Shell_keyUpFunc(void (* callback)(unsigned int keyCode, unsigned int modifierFlags)) {
	keyUpCallback = callback;
}

void Shell_keyModifiersChangedFunc(void (* callback)(unsigned int modifierFlags)) {
	keyModifiersChangedCallback = callback;
}

void Shell_mouseDownFunc(void (* callback)(unsigned int buttonNumber, float x, float y)) {
	mouseDownCallback = callback;
}

void Shell_mouseUpFunc(void (* callback)(unsigned int buttonNumber, float x, float y)) {
	mouseUpCallback = callback;
}

void Shell_mouseMovedFunc(void (* callback)(float x, float y)) {
	mouseMovedCallback = callback;
}

void Shell_mouseDraggedFunc(void (* callback)(unsigned int buttonMask, float x, float y)) {
	mouseDraggedCallback = callback;
}

void Shell_scrollWheelFunc(void (* callback)(int deltaX, int deltaY)) {
	scrollWheelCallback = callback;
}

void Shell_backgroundedFunc(void (* callback)(void)) {
	backgroundedCallback = callback;
}

void Shell_foregroundedFunc(void (* callback)(void)) {
	foregroundedCallback = callback;
}

void EAGLShell_openURLFunc(void (* callback)(const char * url)) {
	openURLCallback = callback;
}

void EAGLShell_touchesCancelledFunc(void (* callback)(unsigned int buttonMask)) {
	touchesCancelledCallback = callback;
}

void EAGLShell_accelerometerFunc(void (* callback)(double x, double y, double z)) {
	accelerometerCallback = callback;
}
