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

#import "eaglshell/EAGLView.h"
#import "eaglshell/EAGLShellApplication.h"
#import <QuartzCore/QuartzCore.h>
#include "shell/Target.h"
#include "shell/Shell.h"
#include "eaglshell/EAGLTarget.h"
#include "glgraphics/GLGraphics.h"

#ifndef __IPHONE_3_1
@interface CADisplayLink : NSObject {
@private
  void *_impl;
}
+ (CADisplayLink *)displayLinkWithTarget:(id)target selector:(SEL)sel;
- (void)addToRunLoop:(NSRunLoop *)runloop forMode:(NSString *)mode;
- (void)removeFromRunLoop:(NSRunLoop *)runloop forMode:(NSString *)mode;
- (void)invalidate;
@property(readonly, nonatomic) CFTimeInterval timestamp, duration;
@property(getter=isPaused, nonatomic) BOOL paused;
@property(nonatomic) NSInteger frameInterval;
@end
#endif

#ifndef __IPHONE_4_0
@interface UIScreen ()
@property(nonatomic,readonly) CGFloat scale;
@end
@interface UIView ()
@property(nonatomic) CGFloat contentScaleFactor;
@end
#endif

@interface EAGLView ()
- (unsigned int) firstAvailableButtonNumber;
@end

@implementation EAGLView

+ (Class) layerClass {
	return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame {
	if ((self = [super initWithFrame: frame]) != nil) {
		CAEAGLLayer * layer;
		
		if ([[[UIDevice currentDevice] systemVersion] compare: @"4.0" options: NSNumericSearch] != NSOrderedAscending) {
			isVersion4OrGreater = true;
			self.contentScaleFactor = [UIScreen mainScreen].scale;
		} else {
			isVersion4OrGreater = false;
		}
		
		self.multipleTouchEnabled = YES;
		
		configuration.preferredOpenGLAPIVersion = EAGLShellOpenGLVersion_ES1 | EAGLShellOpenGLVersion_ES2;
		configuration.displayMode.retainedBacking = false;
		configuration.displayMode.depthAttachment = false;
		configuration.displayMode.stencilAttachment = false;
		configuration.displayMode.colorPrecision = 32;
		configuration.displayMode.depthPrecision = 16;
		configuration.displayMode.packedDepthAndStencil = false;
		
		EAGLTarget_configure(g_argc, g_argv, &configuration);
		
		layer = (CAEAGLLayer *) self.layer;
		layer.opaque = YES;
		layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
			[NSNumber numberWithBool: configuration.displayMode.retainedBacking], kEAGLDrawablePropertyRetainedBacking,
			configuration.displayMode.colorPrecision == 16 ? kEAGLColorFormatRGB565 : kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
		nil];
		
		if (configuration.preferredOpenGLAPIVersion & EAGLShellOpenGLVersion_ES2) {
			context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES2];
			if (context != nil) {
				chosenOpenGLVersion = EAGLShellOpenGLVersion_ES2;
			}
		}
		if (configuration.preferredOpenGLAPIVersion & EAGLShellOpenGLVersion_ES1 && context == nil) {
			context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES1];
			if (context != nil) {
				chosenOpenGLVersion = EAGLShellOpenGLVersion_ES1;
			}
		}
		
		if (context == nil) {
			[self release];
			return nil;
		}
		
		[EAGLContext setCurrentContext: context];
		glGenFramebuffersOES(1, &framebuffer);
		glGenRenderbuffersOES(1, &renderbuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER, framebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER, renderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
		
		if (configuration.displayMode.depthAttachment &&
		    configuration.displayMode.stencilAttachment &&
		    configuration.displayMode.depthPrecision == 24 &&
		    configuration.displayMode.packedDepthAndStencil) {
			glGenRenderbuffersOES(1, &packedDepthStencilBuffer);
			glBindRenderbufferOES(GL_RENDERBUFFER, packedDepthStencilBuffer);
			glFramebufferRenderbufferOES(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_OES, GL_RENDERBUFFER, packedDepthStencilBuffer);
			
		} else {
			if (configuration.displayMode.depthAttachment) {
				glGenRenderbuffersOES(1, &depthBuffer);
				glBindRenderbufferOES(GL_RENDERBUFFER, depthBuffer);
				glFramebufferRenderbufferOES(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
			}
			if (configuration.displayMode.stencilAttachment) {
				glGenRenderbuffersOES(1, &stencilBuffer);
				glBindRenderbufferOES(GL_RENDERBUFFER, stencilBuffer);
				glFramebufferRenderbufferOES(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer);
			}
		}
		
		GLGraphics_init(chosenOpenGLVersion == EAGLShellOpenGLVersion_ES2 ? GL_API_VERSION_ES2 : GL_API_VERSION_ES1);
		
		animating = NO;
		displayLinkSupported = [[[UIDevice currentDevice] systemVersion] compare: @"3.1" options: NSNumericSearch] != NSOrderedAscending;
		displayLink = nil;
		animationTimer = nil;
		
		lastResizeWidth = [UIScreen mainScreen].bounds.size.width;
		lastResizeHeight = [UIScreen mainScreen].bounds.size.height;
		
		activeTouchCount = 0;
	}
	return self;
}

- (void) dealloc {
	[self stopAnimation];
	glDeleteFramebuffersOES(1, &framebuffer);
	glDeleteRenderbuffersOES(1, &renderbuffer);
	if (depthBuffer != 0) {
		glDeleteRenderbuffersOES(1, &depthBuffer);
	}
	if (stencilBuffer != 0) {
		glDeleteRenderbuffersOES(1, &stencilBuffer);
	}
	if (packedDepthStencilBuffer != 0) {
		glDeleteRenderbuffersOES(1, &packedDepthStencilBuffer);
	}
	[context release];
	[super dealloc];
}

- (void) layoutSubviews {
	GLint backingWidth;
	GLint backingHeight;
	
	[EAGLContext setCurrentContext: context];
	glBindRenderbufferOES(GL_RENDERBUFFER, renderbuffer);
	if (!setRenderbufferStorage) {
		// For some reason, doing this more than once causes framebuffer garbage and crashes
		[context renderbufferStorage: GL_RENDERBUFFER fromDrawable: (CAEAGLLayer *) self.layer];
		setRenderbufferStorage = YES;
	}
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	
	if (depthBuffer != 0) {
		glBindRenderbufferOES(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER, configuration.displayMode.depthPrecision == 24 ? GL_DEPTH_COMPONENT24_OES : GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
	}
	if (stencilBuffer != 0) {
		glBindRenderbufferOES(GL_RENDERBUFFER, stencilBuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER, GL_STENCIL_INDEX8, backingWidth, backingHeight);
	}
	if (packedDepthStencilBuffer != 0) {
		glBindRenderbufferOES(GL_RENDERBUFFER, packedDepthStencilBuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, backingWidth, backingHeight);
	}
	
	glViewport(0, 0, backingWidth, backingHeight);
	
	if (backingWidth != lastResizeWidth || backingHeight != lastResizeHeight) {
		lastResizeWidth = backingWidth;
		lastResizeHeight = backingHeight;
		Target_resized(backingWidth, backingHeight);
	}
	
	glBindFramebufferOES(GL_FRAMEBUFFER, framebuffer);
	
	if (Target_draw()) {
		glBindRenderbufferOES(GL_RENDERBUFFER, renderbuffer);
		[context presentRenderbuffer: GL_RENDERBUFFER];
	}
}

- (enum EAGLShellOpenGLVersion) chosenOpenGLVersion {
	return chosenOpenGLVersion;
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
		if (displayLinkSupported) {
			displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget: self selector: @selector(drawView:)];
			[displayLink setFrameInterval: 1];
			[displayLink addToRunLoop: [NSRunLoop currentRunLoop] forMode: NSDefaultRunLoopMode];
		} else {
			animationTimer = [NSTimer scheduledTimerWithTimeInterval: 1.0 / 60.0 target: self selector: @selector(drawView:) userInfo: nil repeats: YES];
		}
		animating = YES;
	}
}

- (void) stopAnimation {
	if (animating) {
		if (displayLinkSupported) {
			[displayLink invalidate];
			displayLink = nil;
		} else {
			[animationTimer invalidate];
			animationTimer = nil;
		}
		animating = NO;
	}
}

- (void) drawView: (id) sender {
#ifdef DEBUG
	GLenum error;
#endif
	
	[EAGLContext setCurrentContext: context];
	glBindFramebufferOES(GL_FRAMEBUFFER, framebuffer);
	
	if (Target_draw()) {
		glBindRenderbufferOES(GL_RENDERBUFFER, renderbuffer);
		[context presentRenderbuffer: GL_RENDERBUFFER];
	}
	
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
}

static unsigned int lowestBitIndex(unsigned int value) {
	// See http://graphics.stanford.edu/~seander/bithacks.html#ZerosOnRightModLookup
	
	static const int mod37BitPosition[] = {
	  32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4,
	  7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5,
	  20, 8, 19, 18
	};
	return mod37BitPosition[(-value & value) % 37];
}

- (unsigned int) firstAvailableButtonNumber {
	unsigned int buttonMask = 0;
	unsigned int activeTouchIndex;
	
	for (activeTouchIndex = 0; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
		buttonMask |= 1 << activeTouches[activeTouchIndex].buttonNumber;
	}
	return lowestBitIndex(~buttonMask);
}

- (void) touchesBegan: (NSSet *) touches withEvent: (UIEvent *) event {
	NSArray * allTouches;
	unsigned int touchIndex;
	
	allTouches = [touches allObjects];
	for (touchIndex = 0; touchIndex < [allTouches count] && activeTouchCount < ACTIVE_TOUCH_MAX; touchIndex++) {
		activeTouches[activeTouchCount].touch = [allTouches objectAtIndex: touchIndex];
		activeTouches[activeTouchCount].lastLocation = [activeTouches[activeTouchCount].touch locationInView: self];
		if (isVersion4OrGreater) {
			activeTouches[activeTouchCount].lastLocation.x *= self.contentScaleFactor;
			activeTouches[activeTouchCount].lastLocation.y *= self.contentScaleFactor;
		}
		activeTouches[activeTouchCount].buttonNumber = [self firstAvailableButtonNumber];
		activeTouchCount++;
		
		Target_mouseDown(activeTouches[activeTouchCount - 1].buttonNumber, activeTouches[activeTouchCount - 1].lastLocation.x, activeTouches[activeTouchCount - 1].lastLocation.y);
	}
}

- (void) touchesMoved: (NSSet *) touches withEvent: (UIEvent *) event {
	NSArray * allTouches;
	unsigned int touchIndex, activeTouchIndex;
	UITouch * touch;
	CGPoint location;
	
	allTouches = [touches allObjects];
	for (touchIndex = 0; touchIndex < [allTouches count]; touchIndex++) {
		touch = [allTouches objectAtIndex: touchIndex];
		location = [touch locationInView: self];
		
		if (isVersion4OrGreater) {
			location.x *= self.contentScaleFactor;
			location.y *= self.contentScaleFactor;
		}
		
		for (activeTouchIndex = 0; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
			if (activeTouches[activeTouchIndex].touch == touch && !CGPointEqualToPoint(location, activeTouches[activeTouchIndex].lastLocation)) {
				activeTouches[activeTouchIndex].lastLocation = location;
				
				Target_mouseDragged(1 << activeTouches[activeTouchIndex].buttonNumber, location.x, location.y);
				break;
			}
		}
	}
}

- (void) touchesEnded: (NSSet *) touches withEvent: (UIEvent *) event {
	NSArray * allTouches;
	unsigned int touchIndex, activeTouchIndex;
	UITouch * touch;
	CGPoint location;
	
	allTouches = [touches allObjects];
	for (touchIndex = 0; touchIndex < [allTouches count]; touchIndex++) {
		touch = [allTouches objectAtIndex: touchIndex];
		location = [touch locationInView: self];
		
		if (isVersion4OrGreater) {
			location.x *= self.contentScaleFactor;
			location.y *= self.contentScaleFactor;
		}
		
		for (activeTouchIndex = 0; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
			if (activeTouches[activeTouchIndex].touch == touch) {
				unsigned int buttonNumber;
				
				buttonNumber = activeTouches[activeTouchIndex].buttonNumber;
				activeTouchCount--;
				for (; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
					activeTouches[activeTouchIndex] = activeTouches[activeTouchIndex + 1];
				}
				
				Target_mouseUp(buttonNumber, location.x, location.y);
				break;
			}
		}
	}
}

- (void) touchesCancelled: (NSSet *) touches withEvent: (UIEvent *) event {
	NSArray * allTouches;
	unsigned int touchIndex, activeTouchIndex;
	UITouch * touch;
	unsigned int buttonMask = 0;
	
	allTouches = [touches allObjects];
	for (touchIndex = 0; touchIndex < [allTouches count]; touchIndex++) {
		touch = [allTouches objectAtIndex: touchIndex];
		
		for (activeTouchIndex = 0; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
			if (activeTouches[activeTouchIndex].touch == touch) {
				buttonMask |= 1 << activeTouches[activeTouchIndex].buttonNumber;
				activeTouchCount--;
				for (; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
					activeTouches[activeTouchIndex] = activeTouches[activeTouchIndex + 1];
				}
				break;
			}
		}
	}
	
	if (buttonMask != 0) {
		EAGLTarget_touchesCancelled(buttonMask);
	}
}

@end
