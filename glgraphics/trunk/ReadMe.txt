GLGraphics provides some miscellaneous general-purpose OpenGL utility code. Included modules:

	* Camera is a StemObject that models a viewpoint in 3D space.

	* GLGraphics defines GLGraphics_init, which is to be called at startup (by the shell, if applicable) to initialize glew (if applicable) and define the OpenGL API version, which can later be retrieved with GLGraphics_getOpenGLAPIVersion.

	* GLIncludes.h is a convenience header that #includes the appropriate OpenGL headers for the target platform.

	* GLUtilities provides OpenGL utility functions for translating between string representations and GLenum representations of OpenGL constants.