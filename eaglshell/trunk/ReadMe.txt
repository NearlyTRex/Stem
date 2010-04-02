EAGLShell is an implementation of a stem shell using EAGL, that builds for iPhone Simulator and iPhone OS. It is a feature complete, production-ready shell.



EAGLShell includes some extensions to the standard shell interface. See EAGLShell.h for details. The target interface remains the same, though you should note:

 - Touch events are processed as mouse events. Each finger generates a Target_mouseDown() with a unique ID buttonNumber for multiple fingers at a time. Target_mouseDragged() is called once per finger that moved, with exactly one bit in buttonMask set each time. Target_mouseUp() is called for each finger lifted. Target_mouseMoved() is never called.

 - Touch events can be canceled. This means it's possible that you'll get calls to Target_mouseDown() and Target_mouseDragged() without matching calls to Target_mouseUp().

 - In order to get keyboard events, you must call EAGLShell_showKeyboard() to bring up the onscreen keyboard. Due to API limitations imposed by Apple, key codes passed to Target_keyUp() are not generally trustworthy for any keyboard other than US English, though character codes are reliable. Target_keyDown() is never called.



Other implementation details to be aware of:

 - Shell_setFullScreen() toggles the visibility of the application status bar.

 - The application's main loop is already executing when Target_init() is called. For consistency's sake, if you do not call Shell_mainLoop() from within Target_init(), the application will be terminated immediately afterward.

 - You must call EAGLShell_setBatteryMonitoringEnabled(true) if you want to get anything useful from Shell_getBatteryState() and Shell_getBatteryLevel().
 
 - EAGLShell_setOrientation() does not transform your OpenGL coordinate space. It is your responsibility to do so if necessary.