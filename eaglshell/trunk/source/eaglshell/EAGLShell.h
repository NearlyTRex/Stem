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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __EAGLShell_H__
#define __EAGLShell_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

enum EAGLShellOrientation {
	EAGLShellOrientation_deviceUpright,
	EAGLShellOrientation_deviceRotatedClockwise,
	EAGLShellOrientation_deviceRotatedCounterclockwise,
	EAGLShellOrientation_deviceUpsideDown
};

enum EAGLShellOpenGLVersion {
	EAGLShellOpenGLVersion_invalid = 0x0,
	EAGLShellOpenGLVersion_ES1 = 0x1,
	EAGLShellOpenGLVersion_ES2 = 0x2
};

enum EAGLShellUIType {
	EAGLShellUIType_iPhone,
	EAGLShellUIType_iPad
};

struct EAGLShellConfiguration {
	/* Your EAGLTarget_configure() function can change this value to express a preference for the
	   OpenGL API version to be used. If your code is compatible with multiple OpenGL API versions,
	   you can use the bitwise OR operator to combine multiple options. The default value is
	   EAGLShellOpenGLVersion_ES1 | EAGLShellOpenGLVersion_ES2. To find out which version was chosen,
	   you can call EAGLShell_getOpenGLAPIVersion() any time after Target_init() has been called.
	   If your requested version is unavailable (for example, requesting EAGLShellOpenGLVersion_ES2
	   on PowerVR MBX hardware), exit() will be called. */
	enum EAGLShellOpenGLVersion preferredOpenGLAPIVersion;
	
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

/** Returns the OpenGL API version that was chosen at initialization time. If you requested more
    than one version in your EAGLShellConfiguration struct, you should call this function after
    Target_init() has been called to determine which version was actually chosen. */
enum EAGLShellOpenGLVersion EAGLShell_getOpenGLAPIVersion();

/** Returns a constant indicating the user interface type of the current device. This is analogous
    to (and implemented with) -[UIDevice userInterfaceIdiom]. */
enum EAGLShellUIType EAGLShell_getUIType();

/** Shows the onscreen keyboard on the appropriate side of the screen based on the application's
    orientation. */
void EAGLShell_showKeyboard();

/** Hides the onscreen keyboard. */
void EAGLShell_hideKeyboard();

/** Sets the application's orientation, which affects the position and orientation of the onscreen
    keyboard if shown (via EAGLShell_showKeyboard()) and the application status bar if shown (via
    Shell_setFullScreen(false)). OpenGL coordinates are not transformed; this is the responsibility
    of your drawing code. */
void EAGLShell_setOrientation(enum EAGLShellOrientation orientation);

/** Sets the enabled state of battery monitoring, which is disabled by default. You must call
    EAGLShell_setBatteryMonitoringEnabled(true) in order for Shell_getBatteryState() to return
    anything other than ShellBatteryState_unknown. */
void EAGLShell_setBatteryMonitoringEnabled(bool enabled);

/** Sets the interval, in seconds, at which EAGLTarget_accelerometer() will be called. Disables
    accelerometer input if zero or a negative number is passed. Accelerometer input is disabled by
    default. */
void EAGLShell_setAccelerometerInterval(double interval);

/** This function can be called at any time to cause all future data written to stdout to be appended
    to a file in the application's sandboxed Documents directory named "stdout". Once called, this
    change will persist until the application is terminated. Primarily for debugging. */
void EAGLShell_redirectStdoutToFile();

#ifdef __cplusplus
}
#endif
#endif
