GLGraphics provides some miscellaneous general-purpose OpenGL utility code. Included modules:

	* GLGraphics defines GLGraphics_init, which is to be called at startup (by the shell, if applicable) to initialize glew (if applicable) and define the OpenGL API version, which can later be retrieved with GLGraphics_getOpenGLAPIVersion.

	* GLIncludes.h is a convenience header that #includes the appropriate OpenGL headers for the target platform.

	* GLSLShader provides simplified vertex and fragment shader setup.

	* GLUtilities provides OpenGL utility functions for translating between string representations and GLenum representations of OpenGL constants.