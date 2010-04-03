#ifndef __EAGLSHELL_H__
#define __EAGLSHELL_H__

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

/** Returns the OpenGL API version that was chosen at initialization time. If your implementation of
    EAGLTarget_getPreferredOpenGLAPIVersion() requests more than one version, you should call
    EAGLShell_getOpenGLAPIVersion() to determine which one was actually chosen. */
enum EAGLShellOpenGLVersion EAGLShell_getOpenGLAPIVersion();

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

#endif
