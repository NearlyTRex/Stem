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
  
  Alex Diener adiener@sacredsoftware.net
*/

#ifndef __GLUT_SHELL_H__
#define __GLUT_SHELL_H__

enum GLUTShellCursor {
	GLUTShellCursor_rightArrow = 100,
	GLUTShellCursor_destroy,
	GLUTShellCursor_help,
	GLUTShellCursor_cycle,
	GLUTShellCursor_spray,
	GLUTShellCursor_upDown,
	GLUTShellCursor_leftRight,
	GLUTShellCursor_topSide,
	GLUTShellCursor_bottomSide,
	GLUTShellCursor_leftSide,
	GLUTShellCursor_rightSide,
	GLUTShellCursor_topLeftCorner,
	GLUTShellCursor_topRightCorner,
	GLUTShellCursor_bottomRightCorner,
	GLUTShellCursor_bottomLeftCorner,
	GLUTShellCursor_inherit,
	GLUTShellCursor_fullCrosshair
};

// Enables/disables vsync for the display mode specified by fullscreen.
// Defaults: Enabled for both on Mac OS X and Windows; enabled only for fullscreen on Linux.
void GLUTShell_setVSync(bool sync, bool fullscreen);

#endif
