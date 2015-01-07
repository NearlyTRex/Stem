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
