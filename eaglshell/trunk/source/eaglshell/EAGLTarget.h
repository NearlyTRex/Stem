/*
  Copyright (c) 2013 Alex Diener
  
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

#ifndef __EAGL_TARGET_H__
#define __EAGL_TARGET_H__

#include "eaglshell/EAGLShell.h"

/** Called prior to Target_init() to allow the target to express a preference for the OpenGL API to
    be used, pixel format, etc. If you're writing a cross-platform target, the fact that
    EAGLTarget_configure() is called allows you to determine at runtime that you're executing on an
    iPhone platform. */
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration);

/** Called when -application:handleOpenURL: is received. If you define CFBundleURLTypes in Info.plist,
    you can register a custom URL scheme that will open your app when such a URL is loaded. Return
    true if the URL was handled successfully. Note that if this function is called, it will be called
    BEFORE Target_init(), so you may want to save the results of parsing the URL for later handling.
    However, you shouldn't save the url pointer itself, because it will be released immediately after
    this function is invoked. */
void EAGLTarget_openURL(const char * url);

/** Called when touch events have been cancelled, meaning touches that were in progress should no
    longer be handled. A bit in buttonMask is set indicating which touches have been canceled,
    corresponding to the buttonMask passed to Target_mouseDragged(). */
void EAGLTarget_touchesCancelled(unsigned int buttonMask);

/** Called at regular intervals if accelerometer events are enabled. To enable accelerometer events,
    call EAGLShell_setAccelerometerInterval() with a positive number for the interval. */
void EAGLTarget_accelerometer(double x, double y, double z);

#endif
