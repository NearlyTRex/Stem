GLUTShell is an implementation of a stem shell using GLUT, that builds for Mac OS X, Windows, and Linux. Although it is not recommended that this shell be used for final products, it is reasonably complete, and suitable for test/example projects.

Due to GLUT limitations, key codes are guessed by mapping from character codes. As a consequence, key codes passed to Target_keyDown and Target_keyUp may be incorrect if the system keyboard layout is something other than en-US.

Building on Windows requires freeglut (http://freeglut.sourceforge.net/) to be added to your MinGW installation. A pre-built freeglut for MinGW is available at http://www.transmissionzero.co.uk/software/freeglut-devel/