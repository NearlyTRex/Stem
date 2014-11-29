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

#ifndef __ShellCallbacks_H__
#define __ShellCallbacks_H__
#ifdef __cplusplus
extern "C" {
#endif

/** Main entry point. Called when the shell's bootstrapping has finished, and a valid OpenGL context
    has been created and made active if the shell uses OpenGL. Call Shell_mainLoop from this function
    if you want to continue execution. If you do not, the program will exit after Target_init() returns.
    You may also want to call the Shell_*Func() functions to set up callbacks for user events. */
void Target_init();

/** Sets function to be called by the shell when a frame is ready to be drawn. In OpenGL-based shells,
    the drawing context will have been made current, and you should issue OpenGL calls to draw your
    scene as appropriate.
    If for some reason you want to cancel drawing and not swap buffers after this function completes,
    return false; otherwise, return true. Call Shell_redisplay() at the end of this function if you
    want it to be called again on the next vertical blank. */
void Shell_drawFunc(bool (* callback)(void));

/** Sets function to be called when the dimensions of the available drawing/input surface have
    changed, such as by a window resize or full-screen toggle. */
void Shell_resizeFunc(void (* callback)(unsigned int newWidth, unsigned int newHeight));

/** Sets function to be called when the user presses a key. charCode is a UTF-32 character value.
    keyCode is one of the KEYBOARD_* or KEYPAD_* values defined by ShellKeyCodes.h. modifierFlags
    is a bitfield containing zero or more of the MODIFIER_* bits defined by ShellKeyCodes.h. */
void Shell_keyDownFunc(void (* callback)(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags));

/** Sets function to be called when the user releases a key. keyCode is one of the KEYBOARD_* or KEYPAD_* values defined
    by ShellKeyCodes.h. modifierFlags is a bitfield containing zero or more of the MODIFIER_* bits
    defined by ShellKeyCodes.h. */
void Shell_keyUpFunc(void (* callback)(unsigned int keyCode, unsigned int modifierFlags));

/** Sets function to be called when the user presses or releases a modifier key. modifierFlags
    is a bitfield containing zero or more of the MODIFIER_* bits defined by ShellKeyCodes.h. */
void Shell_keyModifiersChangedFunc(void (* callback)(unsigned int modifierFlags));

/** Sets function to be called when the user has clicked their mouse or other pointing device.
    For multi-touch events, buttonNumber identifies the finger that has touched the screen,
    starting at 0 and incrementing by 1 for each additional finger. For other pointing devices,
    buttonNumber identifies the button on the pointing device that was pressed, with 0 being
    the primary button. x and y are the pixel coordinates of the click relative to the top left
    corner of the viewport. */
void Shell_mouseDownFunc(void (* callback)(unsigned int buttonNumber, float x, float y));

/** Sets function to be called when the user has released a mouse button or ended a touch.
    For multi-touch events, buttonNumber identifies the finger that has touched the screen,
    starting at 0 and incrementing by 1 for each additional finger. For other pointing devices,
    buttonNumber identifies the button on the pointing device that was pressed, with 0 being
    the primary button. x and y are the pixel coordinates of the click relative to the top left
    corner of the viewport. */
void Shell_mouseUpFunc(void (* callback)(unsigned int buttonNumber, float x, float y));

/** Sets function to be called when the user has moved their mouse cursor without any buttons
    pressed. Not applicable to touch screen devices. If mouse delta mode is active, x and y are
    pixel offsets from the previous mouse position. Otherwise, they are the current pixel
    location of the cursor relative to the top left corner of the viewport. */
void Shell_mouseMovedFunc(void (* callback)(float x, float y));

/** Sets function to be called when the user has moved their mouse cursor, or finger in the case
    of a touch screen device. For each button currently held, the bit at 1 << buttonNumber (as
    specified to the mouseDownFunc callback) in buttonMask will be set. For multi-touch devices,
    when multiple fingers are being moved across the screen, the callback will be invoked once
    per finger moved with a single bit in buttonMask set each time. If mouse delta mode is active,
    x and y are pixel offsets from the previous mouse position. Otherwise, they are the current
    pixel location of the cursor relative to the top left corner of the viewport. */
void Shell_mouseDraggedFunc(void (* callback)(unsigned int buttonMask, float x, float y));

/** Sets function to be called when the user has scrolled with their mouse wheel. deltaX and
    deltaY indicate units of scrolling motion in either direction. The scale of these units may
    vary between different pointing devices, but should correspond roughly to one discrete
    movement of a traditional scroll wheel. */
void Shell_scrollWheelFunc(void (* callback)(int deltaX, int deltaY));

/** Sets function to be called when your application has been placed in the background by the
    user switching away from it to another application, or by other means. The meaning of this
    may vary by operating system/shell, and some shells may not be able to report it at all. */
void Shell_backgroundedFunc(void (* callback)(void));

/** Sets function to be called when your application has been placed in the foreground by the
    user switching to it from another application, or by other means. The meaning of this may
    vary by operating system/shell, and some shells may not be able to report it at all. */
void Shell_foregroundedFunc(void (* callback)(void));

/** Sets function to be called just prior to quitting the application. Return true to continue
    quitting, or false to continue running.
    
    The sole purpose of this callback is to allow targets to temporarily delay quitting to allow
    the user to save any work they would otherwise lose. Do not free memory inside this callback;
    allow the system to reclaim it on exit. If you return false, call the exit() function as soon
    as possible after saving the user's work, unless the user explicitly chooses to cancel their
    request to quit the application. A call to exit() will not trigger this callback, nor will a
    crash or other unclean termination event. */
bool Shell_quitFunc(void (* callback)(void));

#ifdef __cplusplus
}
#endif
#endif
