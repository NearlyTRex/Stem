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

#ifndef __TARGET_H__
#define __TARGET_H__

/** Main entry point. Called when the shell's bootstrapping has finished. You should call
    Shell_mainLoop from this function if you want to continue execution. If you do not, the program
    will exit after Target_init() returns. */
void Target_init();

/** Called by the shell when a frame is ready to be drawn. In OpenGL-based shells, the drawing context
    will have been made current, and you should issue OpenGL calls to draw your scene as appropriate.
    If for some reason you want to cancel drawing and not swap buffers after this function completes,
    return false; otherwise, return true. Call Shell_redisplay() at the end of this function if you
    want it to be called again on the next vertical blank. */
bool Target_draw();

/** Called when the dimensions of the available drawing/input surface have changed, such as by a
    window resize or full-screen toggle. */
void Target_resized(unsigned int newWidth, unsigned int newHeight);

/** Called when the user presses a key. charCode is a UTF-32 character value. keyCode is one of the
    KEYBOARD_* or KEYPAD_* values defined by ShellKeyCodes.h. modifierFlags is a bitfield containing
    zero or more of the MODIFIER_* bits defined by ShellKeyCodes.h. */
void Target_keyDown(unsigned int charCode, unsigned int keyCode, unsigned int modifierFlags);

/** Called when the user releases a key. keyCode is one of the KEYBOARD_* or KEYPAD_* values defined
    by ShellKeyCodes.h. modifierFlags is a bitfield containing zero or more of the MODIFIER_* bits
    defined by ShellKeyCodes.h. */
void Target_keyUp(unsigned int keyCode, unsigned int modifierFlags);

/** Called when the user presses or releases a modifier key. modifierFlags is a bitfield containing
    zero or more of the MODIFIER_* bits defined by ShellKeyCodes.h. */
void Target_keyModifiersChanged(unsigned int modifierFlags);

/** Called when the user has clicked their mouse or other pointing device. For multi-touch events,
    buttonNumber identifies the finger that has touched the screen, starting at 0 and incrementing by
    1 for each additional finger. For other pointing devices, buttonNumber identifies the button on
    the pointing device that was pressed, with 0 being the primary button. x and y are the pixel
    coordinates of the click relative to the top left corner of the window. */
void Target_mouseDown(unsigned int buttonNumber, float x, float y);

/** Called when the user has released a mouse button or ended a touch. See Target_mouseDown for
    parameter descriptions. */
void Target_mouseUp(unsigned int buttonNumber, float x, float y);

/** Called when the user has moved their mouse cursor without any buttons pressed. Not applicable to
    touchscreen devices. */
void Target_mouseMoved(float x, float y);

/** Called when the user has moved their mouse cursor, or finger in the case of a touchscreen device.
    For each button that's down, the bit at 1 << buttonNumber (as specified to Target_mouseDown) in
    buttonMask will be set. In the case of multi-touch, when multiple fingers are being moved across
    the screen, you'll get one call per finger moved with a single bit in buttonMask set each time.
    See Target_mouseDown for other parameter descriptions. */
void Target_mouseDragged(unsigned int buttonMask, float x, float y);

/** Called when your application has been placed in the background by the user switching away from it
    to another application, or by other means. The meaning of this may vary by operating system/shell,
    and some shells may not be able to report it at all. */
void Target_backgrounded();

/** Called when your application has been placed in the foreground by the user switching to it from
    another application, or by other means. The meaning of this may vary by operating system/shell,
    and some shells may not be able to report it at all. */
void Target_foregrounded();

#endif
