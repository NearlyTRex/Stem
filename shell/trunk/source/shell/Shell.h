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

#ifndef __Shell_H__
#define __Shell_H__
#ifdef __cplusplus
extern "C" {
#endif

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

/** Returns the current time in seconds as a high-resolution monotonically increasing value.
    The starting time is unspecified and not meaningful; this function is intended only for
    measuring intervals. */
double Shell_getCurrentTime();

/** Returns the path to the application's built-in resources directory. This directory should also
    be set as the current working directory at startup by shell implementations, so you can safely
    use relative paths to open files without having to call this function. */
const char * Shell_getResourcePath();

/** Returns a path to a system location appropriate for writing local data. The directory is
    created if it doesn't already exist. This location might not be appropriate for storing
    preferences; for that, use Preferences_getFilePath() from the preferences library. The
    subdirectory argument may be NULL to get the root support directory, but it's strongly
    recommended to pass a non-NULL string if you're writing noninvisible files, because the
    returned path may be a visible location like the user's home directory. */
const char * Shell_getSupportPath(const char * subdirectory);

/** Returns the number of logical displays currently attached. */
unsigned int Shell_getDisplayCount();

/** Returns the index of the display containing the largest portion of the main window. */
unsigned int Shell_getDisplayIndexFromWindow();

/** Returns the offset and dimensions of the specified display. Pass NULL for any out parameters
    you don't need returned. If displayIndex is greater than or equal to the return value of
    Shell_getDisplayCount(), the out parameters will not be modified. Display index 0 is the
    main display. */
void Shell_getDisplayBounds(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight);

/** Attempts to enter full screen mode on the specified display. Returns true if successful.
    Unless the user has otherwise selected a specific display, it is recommended to pass the
    return value of Shell_getDisplayIndexFromWindow() as displayIndex. */
bool Shell_enterFullScreen(unsigned int displayIndex);

/** Exits full screen mode. No effect if Shell_enterFullScreen has not been successfully called. */
void Shell_exitFullScreen();

/** Returns true if the shell is currently running in full screen mode. */
bool Shell_isFullScreen();

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

/** Opens the specified URL string in the user's default web browser. */
void Shell_openURL(const char * url);

/** Enables or disables vsync for the display mode specified by fullscreen. If the application is not
    currently in the specified display mode, the change will be applied next time the mode is changed. */
void Shell_setVSync(bool sync, bool fullscreen);

/** Shows a modal Open File dialog, returning the chosen file path in outFilePath. No more than maxLength
    bytes will be written to outFilePath. If the user cancels the dialog, false is returned, and the
    contents of outFilePath are undefined. basePath can optionally be used to specify the starting directory
    for the dialog. */
bool Shell_openFileDialog(const char * basePath, char * outFilePath, unsigned int maxLength);

/** Shows a modal Save File dialog, returning the chosen file path in outFilePath. No more than maxLength
    bytes will be written to outFilePath. If the user cancels the dialog, false is returned, and the
    contents of outFilePath are undefined. basePath can optionally be used to specify the starting directory
    for the dialog. baseName can optionally be used to set a default name for the file to be saved. */
bool Shell_saveFileDialog(const char * basePath, const char * baseName, char * outFilePath, unsigned int maxLength);

#ifdef __cplusplus
}
#endif
#endif
