/*
  Copyright (c) 2012 Alex Diener
  
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

#ifndef __NSOPENGLSHELL_H__
#define __NSOPENGLSHELL_H__

#include <stdbool.h>

struct NSOpenGLShellConfiguration {
	int windowX;
	int windowY;
	int windowWidth;
	int windowHeight;
	char * windowTitle;
	struct {
		bool doubleBuffer;          // NSOpenGLPFADoubleBuffer
		bool depthBuffer;           // NSOpenGLPFADepthSize > 0
		unsigned int depthBits;     // NSOpenGLPFADepthSize value
		bool stencilBuffer;         // NSOpenGLPFAStencilSize > 0
		unsigned int stencilBits;   // NSOpenGLPFAStencilSize value
		bool accumBuffer;           // NSOpenGLPFAAccumSize > 0
		unsigned int accumBits;     // NSOpenGLPFAAccumSize value
		bool multisample;           // NSOpenGLPFAMultisample
		unsigned int sampleBuffers; // NSOpenGLPFASampleBuffers
		unsigned int samples;       // NSOpenGLPFASamples
	} displayMode;
};

// NSOpenGLShellCursor_<name> == [NSCursor <name>Cursor]
enum NSOpenGLShellCursor {
	NSOpenGLShellCursor_arrow,
	NSOpenGLShellCursor_IBeam,
	NSOpenGLShellCursor_crosshair,
	NSOpenGLShellCursor_closedHand,
	NSOpenGLShellCursor_openHand,
	NSOpenGLShellCursor_pointingHand,
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

/** Shows the cursor if visible is set to true. Hides the cursor if visible is set to false. Has no effect if
    the cursor already has the specified visibility. */
void NSOpenGLShell_setCursorVisible(bool visible);

/** Hides the cursor until the mouse moves, at which point it will be shown again. */
void NSOpenGLShell_hideCursorUntilMouseMoves();

/** Sets the cursor image to the specified value. */
void NSOpenGLShell_setCursor(enum NSOpenGLShellCursor cursor);

#endif
