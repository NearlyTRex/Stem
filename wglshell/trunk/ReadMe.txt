WGLShell is an implementation of a stem shell for Windows using WGL and the Win32 native API. It is a feature complete, production-ready shell.

WGLShell includes some extensions to the standard shell interface. You should #include "wglshell/WGLShell.h" to get the extended interface. See the documentation in WGLShell.h for details.

Targets that build with WGLShell must #include "wglshell/WGLTarget.h" and implement WGLTarget_configure(). See WGLTarget.h for details.