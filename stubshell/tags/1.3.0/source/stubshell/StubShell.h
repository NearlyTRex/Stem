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
extern unsigned int (* StubShellCallback_setTimer)(void * context, double interval, bool repeat, void (* callback)(unsigned int timerID, void * timerContext), void * timerContext);
extern void (* StubShellCallback_cancelTimer)(void * context, unsigned int timerID);
extern void (* StubShellCallback_setCursorVisible)(void * context, bool visible);
extern void (* StubShellCallback_hideCursorUntilMouseMoves)(void * context);
extern void (* StubShellCallback_setCursor)(void * context, int cursor);
extern void (* StubShellCallback_setMouseDeltaMode)(void * context, bool deltaMode);
extern ShellThread (* StubShellCallback_createThread)(void * context, void (* threadFunction)(void * context), void * threadContext);
extern void (* StubShellCallback_exitThread)(void * context, int statusCode);
extern void (* StubShellCallback_cancelThread)(void * context, ShellThread thread);
extern void (* StubShellCallback_joinThread)(void * context, ShellThread thread);
extern ShellThread (* StubShellCallback_getCurrentThread)(void * context);
extern ShellMutex (* StubShellCallback_createMutex)(void * context);
extern void (* StubShellCallback_disposeMutex)(void * context, ShellMutex mutex);
extern void (* StubShellCallback_lockMutex)(void * context, ShellMutex mutex);
extern bool (* StubShellCallback_tryLockMutex)(void * context, ShellMutex mutex);
extern void (* StubShellCallback_unlockMutex)(void * context, ShellMutex mutex);

#endif
