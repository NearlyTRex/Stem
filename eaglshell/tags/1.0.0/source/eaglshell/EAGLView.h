#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include "eaglshell/EAGLShell.h"

#define ACTIVE_TOUCH_MAX 32

@interface EAGLView : UIView {
	EAGLContext * context;
	GLuint framebuffer;
	GLuint depthbuffer;
	GLuint renderbuffer;
	
	BOOL animating;
	BOOL redisplayWasPosted;
	BOOL displayLinkSupported;
	id displayLink;
	NSTimer * animationTimer;
	
	unsigned int activeTouchCount;
	struct {
		UITouch * touch;
		CGPoint lastLocation;
		unsigned int buttonNumber;
	} activeTouches[ACTIVE_TOUCH_MAX];
	
	enum EAGLShellOpenGLVersion chosenOpenGLVersion;
}

- (enum EAGLShellOpenGLVersion) chosenOpenGLVersion;
- (void) redisplayPosted;
- (void) drawView: (id) sender;
- (void) startAnimation;
- (void) stopAnimation;

@end
