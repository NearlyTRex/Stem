#import "eaglshell/EAGLView.h"
#import <QuartzCore/QuartzCore.h>
#include "shell/Target.h"

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
		
		self.multipleTouchEnabled = YES;
		
		layer = (CAEAGLLayer *) self.layer;
		layer.opaque = YES;
		layer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool: NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		// TODO: kEAGLRenderingAPIOpenGLES2
		context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES1];
		
		animating = NO;
		displayLinkSupported = [[[UIDevice currentDevice] systemVersion] compare: @"3.1" options: NSNumericSearch] != NSOrderedAscending;
		displayLinkSupported = NO;
		displayLink = nil;
		animationTimer = nil;
		
		activeTouchCount = 0;
	}
	return self;
}

- (void) dealloc {
	[self stopAnimation];
	glDeleteFramebuffersOES(1, &framebuffer);
	glDeleteRenderbuffersOES(1, &renderbuffer);
	[context release];
	[super dealloc];
}

- (void) layoutSubviews {
	GLint backingWidth;
	GLint backingHeight;
	
	[EAGLContext setCurrentContext: context];
	
	glGenFramebuffersOES(1, &framebuffer);
	glGenRenderbuffersOES(1, &renderbuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
	[context renderbufferStorage: GL_RENDERBUFFER_OES fromDrawable: (CAEAGLLayer *) self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	glGenRenderbuffersOES(1, &depthbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthbuffer);
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
	[EAGLContext setCurrentContext: context];
	if (framebuffer == 0) {
		[self layoutSubviews];
	}
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
	
	Target_draw();
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
	[context presentRenderbuffer: GL_RENDERBUFFER_OES];
	
	if (!redisplayWasPosted) {
		[self stopAnimation];
	}
	redisplayWasPosted = NO;
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
	CGPoint location;
	
	allTouches = [touches allObjects];
	for (touchIndex = 0; touchIndex < [allTouches count]; touchIndex++) {
		touch = [allTouches objectAtIndex: touchIndex];
		location = [touch locationInView: self];
		
		for (activeTouchIndex = 0; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
			if (activeTouches[activeTouchIndex].touch == touch) {
				activeTouchCount--;
				for (; activeTouchIndex < activeTouchCount; activeTouchIndex++) {
					activeTouches[activeTouchIndex] = activeTouches[activeTouchIndex + 1];
				}
				break;
			}
		}
	}
}

@end
