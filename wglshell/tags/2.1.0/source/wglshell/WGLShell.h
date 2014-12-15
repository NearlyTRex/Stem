/*
  Copyright (c) 2014 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com
*/

#ifndef __WGLShell_H__
#define __WGLShell_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// http://msdn.microsoft.com/en-us/library/ms648391(v=VS.85).aspx
enum WGLShellCursor {
	WGLShellCursor_appStarting = 100, // IDC_APPSTARTING
	WGLShellCursor_help,              // IDC_HELP
	WGLShellCursor_no,                // IDC_NO
	WGLShellCursor_sizeAll,           // IDC_SIZEALL
	WGLShellCursor_sizeNESW,          // IDC_SIZENESW
	WGLShellCursor_sizeNS,            // IDC_SIZENS
	WGLShellCursor_sizeNWSE,          // IDC_SIZENWSE
	WGLShellCursor_sizeWE,            // IDC_SIZEWE
	WGLShellCursor_upArrow            // IDC_UPARROW
};

// Causes all data subsequently written to stdout to be directed into the file
// specified by path. Primarily for debugging 64-bit applications where building
// with -mwindows is required.
void WGLShell_redirectStdoutToFile(const char * path);

#ifdef __cplusplus
}
#endif
#endif
