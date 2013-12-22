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

#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdbool.h>

enum ShellCursor {
	ShellCursor_arrow,
	ShellCursor_iBeam,
	ShellCursor_crosshair,
	ShellCursor_hand,
	ShellCursor_wait
};

typedef unsigned int ShellTimer;

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

/** Returns a path to a system location appropriate for writing local data. This location may not be
    appropriate for storing preferences; for that, use Preferences_getFilePath() from the
    preferences library. */
const char * Shell_getSupportPath();

/** Returns the width and height of the main screen in outWidth and outHeight. If you're only
    interested in size on one axis, you can safely pass NULL for the other one. */
void Shell_getMainScreenSize(unsigned int * outWidth, unsigned int * outHeight);

/** Sets up a periodic callback. Timer resolution is not in any way guaranteed and may vary
    by operating system and runtime conditions. The return value of this function can be
    passed to Shell_cancelTimer to halt a timer before the next fire time. If repeat is false,
    the timer will automatically be canceled after the first time it fires. */
ShellTimer Shell_setTimer(double interval, bool repeat, void (* callback)(ShellTimer timerID, void * context), void * context);

/** Stops a timer previously set with Shell_setTimer. The timer is guaranteed never to fire
    after this function is called. */
void Shell_cancelTimer(ShellTimer timerID);

/** Shows the cursor if visible is set to true. Hides the cursor if visible is set to false. Has no effect if
    the cursor already has the specified visibility. */
void Shell_setCursorVisible(bool visible);

/** Hides the cursor until the mouse moves, at which point it will be shown again. */
void Shell_hideCursorUntilMouseMoves();

/** Sets the cursor image to the specified value. Values are defined in the ShellCursor enum.
    Additional values may be defined by specific shell implementations. */
void Shell_setCursor(int cursor);

/** If set to true, mouse movement will not move the cursor onscreen, and x and y values reported
    to Target_mouseMoved() and Target_mouseDragged() will be deltas rather than absolute positions.
    False by default. */
void Shell_setMouseDeltaMode(bool deltaMode);

#endif
