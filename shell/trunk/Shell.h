#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdbool.h>

/** Signals the shell to begin the main application loop. This function MAY or MAY NOT return,
    so you mustn't rely on the execution of any code following the call to Shell_mainLoop(). */
void Shell_mainLoop();

/** Queues a call to Target_draw() on the next vertical blank. */
void Shell_redisplay();

/** Returns true if the shell is running in full screen mode, or false if not. */
bool Shell_isFullScreen();

/** Attempts to set the requested screen mode, if different from the current screen mode. The return
    value indicates success; in some situations, it may not be possible to change to the requested
    screen mode, in which case Shell_setFullScreen returns false. */
bool Shell_setFullScreen(bool fullScreen);

/** Returns the current time in seconds as a high-resolution monotonically increasing value
    from an unspecified starting time. */
double Shell_getCurrentTime();

/** Returns the path to the application's built-in resources directory. */
const char * Shell_getResourcePath();

#endif
