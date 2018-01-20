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

#ifndef __EAGLTarget_H__
#define __EAGLTarget_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "eaglshell/EAGLShell.h"

struct EAGLShellConfiguration {
	/* Your EAGLTarget_configure() function can change this value to express a preference for the
	   OpenGL API version to be used. If your code is compatible with multiple OpenGL API versions,
	   you can use the bitwise OR operator to combine multiple options. The default value is
	   EAGLShellOpenGLVersion_ES1 | EAGLShellOpenGLVersion_ES2. To find out which version was chosen,
	   you can call EAGLShell_getOpenGLAPIVersion() any time after Target_init() has been called.
	   If your requested version is unavailable (for example, requesting EAGLShellOpenGLVersion_ES2
	   on PowerVR MBX hardware), exit() will be called. */
	enum EAGLShellOpenGLVersion preferredOpenGLAPIVersion;
	
	// TODO: Standardize names (ex. depthBuffer instead of depthAttachment)
	struct {
		/* kEAGLDrawablePropertyRetainedBacking */
		bool retainedBacking;
		
		/* Precision specified by depthPrecision field */
		bool depthAttachment;
		
		/* Stencil attachments, if enabled, always have 8 bits of precision */
		bool stencilAttachment;
		
		/* Currently valid values: 16 (RGB 565), 32 (RGBA 8888); all other values will be interpreted
		   as 32 */
		int colorPrecision;
		
		/* Currently valid values: 16, 24 (ignored if depthAttachment is false); all other values will
		   be interpreted as 16 */
		int depthPrecision;
		
		/* If true, depth and stencil attachments are packed into a single buffer. The default value
		   is false. This is ignored if depthAttachment or stencilAttachment is false, or depthPrecision
		   is not 24. */
		bool packedDepthAndStencil;
		
	} displayMode;
};

/** Called prior to Target_init() to allow the target to express a preference for the OpenGL API to
    be used, pixel format, etc. If you're writing a cross-platform target, the fact that
    EAGLTarget_configure() is called allows you to determine at runtime that you're executing on an
    iPhone platform. */
void EAGLTarget_configure(int argc, char ** argv, struct EAGLShellConfiguration * configuration);

/** Sets function to be called when -application:handleOpenURL: is received. If you define
    CFBundleURLTypes in Info.plist, you can register a custom URL scheme that will open your app when
    such a URL is loaded. Return true if the URL was handled successfully. Note that if this function
    is called, it will be called BEFORE Target_init(), so you may want to save the results of parsing
    the URL for later handling. However, you shouldn't save the url pointer itself, because it will
    be released immediately after this function is invoked. */
void EAGLShell_openURLFunc(void (* callback)(const char * url));

/** Sets function to be called when touch events have been cancelled, meaning touches that were in
    progress should no longer be handled. A bit in buttonMask is set indicating which touches have
    been canceled, corresponding to the buttonMask passed to Target_mouseDragged(). */
void EAGLShell_touchesCancelledFunc(void (* callback)(unsigned int buttonMask));

/** Sets function to be called at regular intervals if accelerometer events are enabled. To enable
    accelerometer events, call EAGLShell_setAccelerometerInterval() with a positive number for the
    interval. */
void EAGLShell_accelerometerFunc(void (* callback)(double x, double y, double z));

#ifdef __cplusplus
}
#endif
#endif
