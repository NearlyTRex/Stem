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

#ifndef __NSOpenGLTarget_H__
#define __NSOpenGLTarget_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "nsopenglshell/NSOpenGLShell.h"

struct NSOpenGLShellConfiguration {
	// All coordinate units are pixels, not points. Default sizes for retina displays should be set accordingly.
	int windowX;
	int windowY;
	unsigned int windowWidth;
	unsigned int windowHeight;
	char * windowTitle;
	bool fullScreenMenuItem; // If set to true, the Window menu will contain an Enter Full Screen item which calls Shell_enterFullScreen when selected. Default is false.
	bool vsyncWindowed; // Can be changed after configure by calling NSOpenGLShell_setVSync()
	bool vsyncFullScreen; // Can be changed after configure by calling NSOpenGLShell_setVSync()
	bool useGLCoreProfile; // If set, creates an OpenGL 3.2 Core Profile context
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

/** Called prior to Target_init() to allow the target to express preferences for various settings related
    to OpenGL context setup and other things. See the NSOpenGLShellConfiguration struct in NSOpenGLShell.h
    for details. */
void NSOpenGLTarget_configure(int argc, const char ** argv, struct NSOpenGLShellConfiguration * configuration);

#ifdef __cplusplus
}
#endif
#endif
