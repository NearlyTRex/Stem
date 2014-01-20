GLUTShell is an implementation of a stem shell using GLUT, that builds for Mac OS X, Windows, and Linux. Although it is not recommended that this shell be used for final products, it is reasonably complete, and suitable for test/example projects.

Due to GLUT limitations, key codes are guessed by mapping from character codes. As a consequence, key codes passed to Target_keyDown and Target_keyUp may be incorrect if the system keyboard layout is something other than en-US.

Building on Windows requires freeglut (http://freeglut.sourceforge.net/) to be added to your MinGW installation. A pre-built freeglut for MinGW is available at http://www.transmissionzero.co.uk/software/freeglut-devel/

Note that GLUT cannot detect key modifier changes except while a keyboard or mouse handler callback is being processed, so Target_keyModifiersChanged() will not be called immediately after a modifier key has been pressed; it will only be called prior to Target_keyDown(), Target_keyUp(), Target_mouseDown(), and Target_mouseUp() if the modifier state is different than it was the last time one of those functions was called. Also, only shift, control, and alt will be set; caps lock and command cannot be detected.

Foreground and background events also cannot be detected. Target_foregrounded() and Target_backgrounded() will never be called from GLUTShell.

Shell_openURL() is not implemented by glutshell. Calling it has no effect.