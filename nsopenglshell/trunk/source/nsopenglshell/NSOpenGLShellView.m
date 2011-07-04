/*
  Copyright (c) 2011 Alex Diener
  
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

#import "nsopenglshell/NSOpenGLShellView.h"
#import "nsopenglshell/NSOpenGLShellApplication.h"
#import <OpenGL/glu.h>
#include "shell/Target.h"
#include "shell/Shell.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#include "glgraphics/GLGraphics.h"

@interface NSOpenGLShellView ()
- (void) stopAnimation;
@end

@implementation NSOpenGLShellView

- (id) initWithFrame: (NSRect) frame configuration: (struct NSOpenGLShellConfiguration) configuration {
	NSOpenGLPixelFormat * pixelFormat;
	NSOpenGLPixelFormatAttribute attributes[18];
	GLint sync = 1;
	unsigned int numAttributes = 0;
	
	attributes[numAttributes++] = NSOpenGLPFANoRecovery;
	attributes[numAttributes++] = NSOpenGLPFAScreenMask;
	attributes[numAttributes++] = CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay);
	if (configuration.displayMode.doubleBuffer) {
		attributes[numAttributes++] = NSOpenGLPFADoubleBuffer;
	}
	if (configuration.displayMode.depthBuffer) {
		attributes[numAttributes++] = NSOpenGLPFADepthSize;
		attributes[numAttributes++] = 32; // TODO: Specify in configuration
	}
	if (configuration.displayMode.stencilBuffer) {
		attributes[numAttributes++] = NSOpenGLPFAStencilSize;
		attributes[numAttributes++] = 8; // TODO: Specify in configuration
	}
	if (configuration.displayMode.accumBuffer) {
		attributes[numAttributes++] = NSOpenGLPFAAccumSize;
		attributes[numAttributes++] = 32; // TODO: Specify in configuration
	}
	if (configuration.displayMode.multisample) {
		attributes[numAttributes++] = NSOpenGLPFAMultisample;
		attributes[numAttributes++] = NSOpenGLPFASampleBuffers;
		attributes[numAttributes++] = 1; // TODO: Specify in configuration?
		attributes[numAttributes++] = NSOpenGLPFASamples;
		attributes[numAttributes++] = 4; // TODO: Specify in configuration
	}
	attributes[numAttributes] = 0;
	pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attributes];
	if ((self = [super initWithFrame: frame pixelFormat: pixelFormat]) != nil) {
		[pixelFormat release];
		
		GLGraphics_init(GL_API_VERSION_DESKTOP_1); // TODO!!
		
		attributes[numAttributes++] = NSOpenGLPFAFullScreen;
		attributes[numAttributes] = 0;
		pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attributes];
		fullScreenContext = [[NSOpenGLContext alloc] initWithFormat: pixelFormat shareContext: [self openGLContext]];
		[fullScreenContext setValues: &sync forParameter: NSOpenGLCPSwapInterval];
		[[self openGLContext] setValues: &sync forParameter: NSOpenGLCPSwapInterval];
		
		fullScreen = false;
		animationTimer = nil;
	}
	[pixelFormat release];
	return self;
}

- (void) dealloc {
	[self stopAnimation];
	[fullScreenContext release];
	[super dealloc];
}

#define FADE_INTERVAL 0.25

- (void) toggleFullScreen {
	CGError error;
	CGDisplayFadeReservationToken token;
	
	// TODO (backgrounded?)
	//[controller suspend];
	
	error = CGAcquireDisplayFadeReservation(kCGMaxDisplayReservationInterval, &token);
	if (error == kCGErrorSuccess) {
		CGDisplayFade(token, FADE_INTERVAL, kCGDisplayBlendNormal, kCGDisplayBlendSolidColor, 0.0, 0.0, 0.0, 1);
	}
	
	if (fullScreen) {
		error = CGDisplayRelease((CGDirectDisplayID) [[[[[self window] screen] deviceDescription] objectForKey: @"NSScreenNumber"] longValue]);
		
		fullScreen = false;
    [NSMenu setMenuBarVisible: YES];
		[fullScreenContext clearDrawable];
		[[self openGLContext] makeCurrentContext];
		Target_resized([self bounds].size.width, [self bounds].size.height);
	} else {
		error = CGDisplayCapture((CGDirectDisplayID) [[[[[self window] screen] deviceDescription] objectForKey: @"NSScreenNumber"] longValue]);
		
		[fullScreenContext setFullScreen];
		[fullScreenContext makeCurrentContext];
    [NSMenu setMenuBarVisible: NO];
		fullScreen = true;
	}
	
	[self setNeedsDisplay: YES];
	
	if (error == kCGErrorSuccess) {
		CGDisplayFade(token, FADE_INTERVAL, kCGDisplayBlendSolidColor, kCGDisplayBlendNormal, 0.0, 0.0, 0.0, 0);
		CGReleaseDisplayFadeReservation(token);
	}
	
	// TODO (foregrounded?)
	//[controller resume];
}

- (bool) isFullScreen {
	return fullScreen;
}

- (NSRect) bounds {
	if (fullScreen) {
		return [[NSScreen mainScreen] frame];
	} else {
		return [super bounds];
	}
}

- (void) mouseDown: (NSEvent *) event {
	Target_mouseDown([event buttonNumber], [event locationInWindow].x, [self frame].size.height - [event locationInWindow].y);
}

- (void) mouseUp: (NSEvent *) event {
	Target_mouseUp([event buttonNumber], [event locationInWindow].x, [self frame].size.height - [event locationInWindow].y);
}

- (void) redisplayPosted {
	if (animating) {
		redisplayWasPosted = YES;
	} else {
		[self startAnimation];
	}
}

- (void) startAnimation {
	if (!animating) {
		animationTimer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / 60.0 target: self selector: @selector(drawTimer:) userInfo: nil repeats: YES];
		animating = YES;
	}
}

- (void) stopAnimation {
	if (animating) {
		[animationTimer invalidate];
		animationTimer = nil;
		animating = NO;
	}
}

- (void) drawRect: (NSRect) rect {
#ifdef DEBUG
	GLenum error;
#endif
	
	if (fullScreen) {
		[fullScreenContext makeCurrentContext];
	} else {
		[[self openGLContext] makeCurrentContext];
	}
	
	Target_draw();
	
	if (!redisplayWasPosted) {
		[self stopAnimation];
	}
	redisplayWasPosted = NO;
	
#ifdef DEBUG
	error = glGetError();
	while (error != GL_NO_ERROR) {
		fprintf(stderr, "GL error: 0x%X\n", error);
		error = glGetError();
	}
#endif
	
  if (fullScreen) {
    [fullScreenContext flushBuffer];
  } else {
    [[self openGLContext] flushBuffer];
  }
}

- (void) drawTimer: (NSTimer *) timer {
	[self setNeedsDisplay: YES];
}

- (void) setNeedsDisplay: (BOOL) needsDisplay {
  if (fullScreen) {
    if (needsDisplay) {
    	[self drawRect: [self bounds]];
    }
  } else {
    [super setNeedsDisplay: needsDisplay];
  }
}

/*
- (void) setNeedsDisplay: (BOOL) needsDisplay {
	if (fullScreen) {
		if (needsDisplay) {
			Target_draw();
		}
	} else {
		[super setNeedsDisplay: needsDisplay];
	}
}
*/

- (void) reshape {
	NSRect bounds;
	
	bounds = [self bounds];
	glViewport(0, 0, bounds.size.width, bounds.size.height);
	Target_resized(bounds.size.width, bounds.size.height);
}

- (BOOL) isFlipped {
	return YES;
}

- (void) copyGLToBackingStore {
	NSSize size;
	void * buffer;
	
	[[self openGLContext] makeCurrentContext];
	size = [self bounds].size;
	
	buffer = malloc(size.width * size.height * 4);
	glReadPixels(0, 0, size.width, size.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	
	[self lockFocus];
	NSDrawBitmap([self bounds], size.width, size.height, 8, 4, 32, (size.width * 4), NO, NO, NSDeviceRGBColorSpace, (const unsigned char **) &buffer);
	[self unlockFocus];
	
	free(buffer);
	
	[[self window] flushWindow];
}

@end
