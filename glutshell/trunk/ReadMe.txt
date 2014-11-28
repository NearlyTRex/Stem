GLUTShell is an implementation of a stem shell using GLUT for Mac OS X, Windows, and Linux. Although it is not recommended that this shell be used for final products, it is reasonably complete, and suitable for test/example projects.

Due to GLUT limitations, key codes are guessed by mapping from character codes. As a consequence, key codes passed to the Shell_keyDownFunc and Shell_keyUpFunc callbacks may be incorrect if the system keyboard layout is something other than en-US.

Building on Windows requires freeglut (http://freeglut.sourceforge.net/) to be added to your MinGW installation. A pre-built freeglut for MinGW is available at http://www.transmissionzero.co.uk/software/freeglut-devel/

Note that GLUT cannot detect key modifier changes except while a keyboard or mouse handler callback is being processed, so the Shell_keyModifiersChangedFunc callback will not be called immediately after a modifier key has been pressed; it will only be called prior to the Shell_keyDownFunc, Shell_keyUpFunc, Shell_mouseDownFunc, and Shell_mouseUpFunc callbacks if the modifier state is different than it was the last time one of those functions was called. Also, only shift, control, and alt will be set; caps lock and command cannot be detected.

Foreground and background events also cannot be detected. The Shell_foregroundedFunc and Shell_backgroundedFunc callbacks will never be invoked from GLUTShell.

GLUT does not have an official API for mouse wheel events. The Shell_scrollWheelFunc callback will never be invoked.

Shell_openURL() is not implemented by glutshell. Calling it has no effect.

Shell_openFileDialog() and Shell_saveFileDialog() display nothing and always return false. Use a platform-native shell if you want to browse for files.

Shell_systemBeep() has no effect.

Multiple display support is unimplemented. The Shell functions that deal with displays behave in the following way:
- Shell_getDisplayCount() always returns 1
- Shell_getDisplayIndexFromWindow() always returns 0
- Shell_getDisplayBounds() ignores its displayIndex parameter and may return unexpected width/height values for if multiple displays are attached
- Shell_enterFullScreen() ignores its displayIndex parameter and will enter full screen mode on the main display or the display containing the main window
- Shell_getSafeWindowRect() returns the same values as Shell_getDisplayBounds()
