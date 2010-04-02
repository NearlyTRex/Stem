#ifndef __EAGLSHELL_H__
#define __EAGLSHELL_H__

#include <stdbool.h>

enum EAGLShellOrientation {
	EAGLShellOrientation_deviceUpright,
	EAGLShellOrientation_deviceRotatedClockwise,
	EAGLShellOrientation_deviceRotatedCounterclockwise,
	EAGLShellOrientation_deviceUpsideDown
};

void EAGLShell_showKeyboard();
void EAGLShell_hideKeyboard();

void EAGLShell_setBatteryMonitoringEnabled(bool enabled);

void EAGLShell_setOrientation(enum EAGLShellOrientation orientation);

// TODO: Expose accelerometer

#endif
