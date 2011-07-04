NSOpenGLShell is an implementation of a stem shell for Mac OS X using NSOpenGL. It is a feature complete, production-ready shell.

NSOpenGLShell includes some extensions to the standard shell interface. You should #include "nsopenglshell/NSOpenGLShell.h" to get the extended interface. See the documentation in NSOpenGLShell.h for details.

Targets that build with NSOpenGLShell must #include "nsopenglshell/NSOpenGLTarget.h" and implement NSOpenGLTarget_configure(). See NSOpenGLTarget.h for details.