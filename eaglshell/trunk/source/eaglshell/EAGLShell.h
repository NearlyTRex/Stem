#ifndef __EAGLSHELL_H__
#define __EAGLSHELL_H__

#include <stdbool.h>

enum EAGLShellOrientation {
	EAGLShellOrientation_deviceUpright,
	EAGLShellOrientation_deviceRotatedClockwise,
	EAGLShellOrientation_deviceRotatedCounterclockwise,
	EAGLShellOrientation_deviceUpsideDown
};

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
