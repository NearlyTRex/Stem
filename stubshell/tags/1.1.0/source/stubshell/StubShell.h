#ifndef __STUBSHELL_H__
#define __STUBSHELL_H__

#include "shell/ShellBatteryInfo.h"

/** Passed as the first argument to all StubShellCallback_* functions. Defaults to NULL. */
extern void * StubShell_callbackContext;

/** For each Shell function implemented, you can set a callback to be notified when the function is
    called, and specify a return value for functions that return non-void. If set to NULL, a default
    implementation will be used. All callbacks are initially NULL. */
extern void (* StubShellCallback_mainLoop)(void * context);
extern void (* StubShellCallback_redisplay)(void * context);
extern bool (* StubShellCallback_isFullScreen)(void * context);
extern bool (* StubShellCallback_setFullScreen)(void * context, bool fullScreen);
extern double (* StubShellCallback_getCurrentTime)(void * context);
extern const char * (* StubShellCallback_getResourcePath)(void * context);
extern enum ShellBatteryState (* StubShellCallback_getBatteryState)(void * context);
extern float (* StubShellCallback_getBatteryLevel)(void * context);

#endif
