EAGLShell is an implementation of a stem shell using EAGL, that builds for iPhone Simulator and iPhone OS. It is a feature complete, production-ready shell.

EAGLShell includes some extensions to the standard shell interface. You should #include "eaglshell/EAGLShell.h" to get the extended interface. It provides facilities for working with the iPhone's status bar, keyboard, and accelerometer. See the documentation in EAGLShell.h for details. Some things to note:

 - You must express a preference for the OpenGL ES API version to be used by implementing EAGLTarget_getPreferredOpenGLAPIVersion(). You can subsequently call EAGLShell_getOpenGLAPIVersion() to find out which API version was chosen.

 - In order to receive keyboard events, you must call EAGLShell_showKeyboard() to bring up the onscreen keyboard. Due to API limitations imposed by Apple, key codes passed to Target_keyUp() are not generally trustworthy for any keyboard other than US English, though character codes are reliable. Target_keyDown() is never called.

 - EAGLShell_setOrientation() does not transform your OpenGL coordinate space. It is your responsibility to do so if necessary.

 - You must call EAGLShell_setBatteryMonitoringEnabled(true) if you want to get anything useful from Shell_getBatteryState() and Shell_getBatteryLevel().
 
 - Shell_setFullScreen() toggles the visibility of the application status bar.

 - The application's main loop is already executing when Target_init() is called. For consistency's sake, if you do not call Shell_mainLoop() from within Target_init(), exit() is called and your application will terminate.

 - You must call EAGLShell_setAccelerometerInterval() with a nonzero interval to receive accelerometer events. See the target extension notes below for more details.



The target interface has also been extended, with EAGLTarget.h defining the new functions that targets must implement in order to receive notifications of accelerometer acceleration and cancelled touch events. There are some differences in behavior for the standard Target functions, as well:

 - Touch events are processed as mouse events. Each finger generates a Target_mouseDown() with a unique ID buttonNumber for multiple fingers at a time. Target_mouseDragged() is called once per finger that moved, with exactly one bit in buttonMask set each time. Target_mouseUp() is called for each finger lifted. Target_mouseMoved() is never called.

 - Touch events can be cancelled. This means it's possible that you'll get calls to Target_mouseDown() and Target_mouseDragged() without matching calls to Target_mouseUp(). To detect and process cancelled touch events, you must implement EAGLTarget_touchesCancelled().

 - Accelerometer events are delivered via EAGLTarget_accelerometer, at the interval specified to EAGLShell_setAccelerometerInterval().



Other things to be aware of:
 
 - EAGLShell compiles against the 3.1 iPhone SDK. This is due only to bugs and instability in iPhone OS 3.0; it doesn't actually use any OS 3.1 features (except CADisplayLink, which is linked in at runtime if available; if not, a timer is used instead).

 - Due to an Apple bug, as of iPhone OS 3.1.3, Target_resized() will not be called when the status bar changes size due to a phone call or voice recording starting or ending. See http://openradar.appspot.com/6475681 for details.
