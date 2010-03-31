#ifndef __TARGET_H__
#define __TARGET_H__

/** Called by the shell to retrieve a human-readable display name for the target application. */
const char * Target_getName();

/** Main entry point. Called when the shell's bootstrapping has finished. You should call
    Shell_mainLoop from this function if you want to continue execution. argc and argv are passed
    as they were received by main(). */
void Target_init(int argc, char ** argv);

/** Called by the shell when a frame is ready to be drawn. In OpenGL-based shells, the drawing context
    will have been made current, and you should issue OpenGL calls to draw your scene as appropriate.
    This function also serves as the main heartbeat of the target application. Call Shell_redisplay()
    at the end of this function if you want it to be called again on the next vertical blank. */
void Target_draw();

/** Called when the dimensions of the available drawing/input  surface have changed, such as by a
    window resize or full-screen toggle. */
void Target_resized(unsigned int newWidth, unsigned int newHeight);

/** Called when the user presses a key. charCode is a UTF-32 character value. keyCode is a value
    defined by ShellKeyCodes.h. */
void Target_keyDown(unsigned int charCode, unsigned int keyCode);

/** Called when the user releases a key. charCode is a UTF-32 character value. keyCode is a value
    defined by ShellKeyCodes.h. */
void Target_keyUp(unsigned int charCode, unsigned int keyCode);

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
    buttonMask will be set. See Target_mouseDown for other parameter descriptions. */
void Target_mouseDragged(unsigned int buttonMask, float x, float y);

#endif
