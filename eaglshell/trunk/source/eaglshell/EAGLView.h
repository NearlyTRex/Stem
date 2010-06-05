/*
  Copyright (c) 2010 Alex Diener
  
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

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "eaglshell/EAGLShell.h"

#define ACTIVE_TOUCH_MAX 32

@interface EAGLView : UIView {
	EAGLContext * context;
	GLuint framebuffer;
	GLuint depthBuffer;
	GLuint stencilBuffer;
	GLuint packedDepthStencilBuffer;
	GLuint renderbuffer;
	
	BOOL animating;
	BOOL redisplayWasPosted;
	BOOL displayLinkSupported;
	BOOL setRenderbufferStorage;
	id displayLink;
	NSTimer * animationTimer;
	
	int lastResizeWidth;
	int lastResizeHeight;
	
	unsigned int activeTouchCount;
	struct {
		UITouch * touch;
		CGPoint lastLocation;
		unsigned int buttonNumber;
	} activeTouches[ACTIVE_TOUCH_MAX];
	
	struct EAGLShellConfiguration configuration;
	enum EAGLShellOpenGLVersion chosenOpenGLVersion;
}

- (enum EAGLShellOpenGLVersion) chosenOpenGLVersion;
- (void) redisplayPosted;
- (void) drawView: (id) sender;
- (void) startAnimation;
- (void) stopAnimation;

@end
