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

#ifndef __NSOpenGLShell_H__
#define __NSOpenGLShell_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// NSOpenGLShellCursor_<name> == [NSCursor <name>Cursor]
enum NSOpenGLShellCursor {
	NSOpenGLShellCursor_closedHand = 100,
	NSOpenGLShellCursor_openHand,
	NSOpenGLShellCursor_resizeLeft,
	NSOpenGLShellCursor_resizeRight,
	NSOpenGLShellCursor_resizeLeftRight,
	NSOpenGLShellCursor_resizeUp,
	NSOpenGLShellCursor_resizeDown,
	NSOpenGLShellCursor_resizeUpDown,
	NSOpenGLShellCursor_disappearingItem,
	// Available on Mac OS X 10.6 or later only
	NSOpenGLShellCursor_contextualMenu,
	NSOpenGLShellCursor_dragCopy,
	NSOpenGLShellCursor_dragLink,
	NSOpenGLShellCursor_operationNotAllowed
};

#ifdef __cplusplus
}
#endif
#endif
