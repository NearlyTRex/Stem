#ifndef __EAGL_TARGET_H__
#define __EAGL_TARGET_H__

#include "eaglshell/EAGLShell.h"

/** Called at startup to allow the target to express a preference for the OpenGL API to be used.
    You may request multiple versions by combining EAGLShellOpenGLVersion values with the bitwise OR
    operator. If multiple versions are requested, the newest one will be preferred. You can query the
    version chosen by calling EAGLShell_getOpenGLAPIVersion(). */
enum EAGLShellOpenGLVersion EAGLTarget_getPreferredOpenGLAPIVersion();

/** Called when touch events have been cancelled, meaning touches that were in progress should no
    longer be handled. A bit in buttonMask is set indicating which touches have been canceled,
    corresponding to the buttonMask passed to Target_mouseDragged(). */
void EAGLTarget_touchesCancelled(unsigned int buttonMask);

/** Called at regular intervals if accelerometer events are enabled. To enable accelerometer events,
    call EAGLShell_setAccelerometerInterval() with a positive number for the interval. */
void EAGLTarget_accelerometer(double x, double y, double z);

#endif
