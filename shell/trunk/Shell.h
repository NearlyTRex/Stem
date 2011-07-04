/*
  Copyright (c) 2011 Alex Diener
  
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

/** Returns the current time in seconds as a high-resolution monotonically increasing value.
    The starting time is unspecified and not meaningful; this function is intended only for
    measuring intervals. */
double Shell_getCurrentTime();

/** Returns the path to the application's built-in resources directory. This directory should also
    be set as the current working directory at startup by shell implementations, so you can safely
    use relative paths to open files without having to call this function. */
const char * Shell_getResourcePath();

#endif
