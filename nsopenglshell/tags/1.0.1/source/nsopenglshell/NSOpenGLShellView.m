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
#include "glgraphics/GLGraphics.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#include "shell/Target.h"
#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include <OpenGL/glu.h>

@interface NSOpenGLShellView ()
- (void) stopAnimation;
@end

@implementation NSOpenGLShellView

- (id) initWithFrame: (NSRect) frame configuration: (struct NSOpenGLShellConfiguration) configuration {
	NSOpenGLPixelFormat * pixelFormat;
	NSOpenGLPixelFormatAttribute attributes[13];
	GLint sync = 1;
	unsigned int numAttributes = 0;
	
	if (configuration.displayMode.doubleBuffer) {
		attributes[numAttributes++] = NSOpenGLPFADoubleBuffer;
	}
	if (configuration.displayMode.depthBuffer) {
		attributes[numAttributes++] = NSOpenGLPFADepthSize;
		attributes[numAttributes++] = configuration.displayMode.depthBits;
	}
	if (configuration.displayMode.stencilBuffer) {
		attributes[numAttributes++] = NSOpenGLPFAStencilSize;
		attributes[numAttributes++] = configuration.displayMode.stencilBits;
	}
	if (configuration.displayMode.accumBuffer) {
		attributes[numAttributes++] = NSOpenGLPFAAccumSize;
		attributes[numAttributes++] = configuration.displayMode.accumBits;
	}
	if (configuration.displayMode.multisample) {
		attributes[numAttributes++] = NSOpenGLPFAMultisample;
		attributes[numAttributes++] = NSOpenGLPFASampleBuffers;
		attributes[numAttributes++] = configuration.displayMode.sampleBuffers;
		attributes[numAttributes++] = NSOpenGLPFASamples;
		attributes[numAttributes++] = configuration.displayMode.samples;
	}
	attributes[numAttributes] = 0;
	pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attributes];
	if ((self = [super initWithFrame: frame pixelFormat: pixelFormat]) != nil) {
		const unsigned char * glExtensions;
		
		[pixelFormat release];
		
		[[self openGLContext] makeCurrentContext];
		glExtensions = glGetString(GL_EXTENSIONS);
		if (!gluCheckExtension((unsigned char *) "GL_ARB_shader_objects", glExtensions) || strstr("GMA", (char *) glGetString(GL_RENDERER))) {
			GLGraphics_init(GL_API_VERSION_DESKTOP_1);
		} else {
			GLGraphics_init(GL_API_VERSION_DESKTOP_2);
		}
		
		[[self openGLContext] setValues: &sync forParameter: NSOpenGLCPSwapInterval];
		
		animationTimer = nil;
	}
	[pixelFormat release];
	return self;
}

- (void) dealloc {
	[self stopAnimation];
	[super dealloc];
}

#define FADE_INTERVAL 0.25

- (void) toggleFullScreen {
	if ([self isInFullScreenMode]) {
		[self exitFullScreenModeWithOptions: nil];
	} else {
		[self enterFullScreenMode: [[self window] screen] withOptions: nil];
	}
	[[self window] makeFirstResponder: self];
}

- (void) mouseMoved: (NSEvent *) event {
	Target_mouseMoved([event locationInWindow].x, [self frame].size.height - [event locationInWindow].y);
}

- (void) mouseDown: (NSEvent *) event {
	buttonMask |= 1 << [event buttonNumber];
	Target_mouseDown([event buttonNumber], [event locationInWindow].x, [self frame].size.height - [event locationInWindow].y);
}

- (void) mouseDragged: (NSEvent *) event {
	Target_mouseDragged(buttonMask, [event locationInWindow].x, [self frame].size.height - [event locationInWindow].y);
}

- (void) mouseUp: (NSEvent *) event {
	buttonMask &= ~(1 << [event buttonNumber]);
	Target_mouseUp([event buttonNumber], [event locationInWindow].x, [self frame].size.height - [event locationInWindow].y);
}

- (void) rightMouseDown: (NSEvent *) event {
	[self mouseDown: event];
}

- (void) rightMouseDragged: (NSEvent *) event {
	[self mouseDragged: event];
}

- (void) rightMouseUp: (NSEvent *) event {
	[self mouseUp: event];
}

- (void) otherMouseDown: (NSEvent *) event {
	[self mouseDown: event];
}

- (void) otherMouseDragged: (NSEvent *) event {
	[self mouseDragged: event];
}

- (void) otherMouseUp: (NSEvent *) event {
	[self mouseUp: event];
}

static unsigned int NSEventKeyCodeToShellKeyCode(unsigned short keyCode) {
	switch (keyCode) {
		case 53:
			return KEYBOARD_ESCAPE;
		case 122:
			return KEYBOARD_F1;
		case 120:
			return KEYBOARD_F2;
		case 99:
			return KEYBOARD_F3;
		case 118:
			return KEYBOARD_F4;
		case 96:
			return KEYBOARD_F5;
		case 97:
			return KEYBOARD_F6;
		case 98:
			return KEYBOARD_F7;
		case 100:
			return KEYBOARD_F8;
		case 101:
			return KEYBOARD_F9;
		case 109:
			return KEYBOARD_F10;
		case 103:
			return KEYBOARD_F11;
		case 111:
			return KEYBOARD_F12;
		case 105:
			return KEYBOARD_F13;
		case 107:
			return KEYBOARD_F14;
		case 113:
			return KEYBOARD_F15;
		case 50:
			return KEYBOARD_GRAVE_ACCENT_AND_TILDE;
		case 18:
			return KEYBOARD_1;
		case 19:
			return KEYBOARD_2;
		case 20:
			return KEYBOARD_3;
		case 21:
			return KEYBOARD_4;
		case 23:
			return KEYBOARD_5;
		case 22:
			return KEYBOARD_6;
		case 26:
			return KEYBOARD_7;
		case 28:
			return KEYBOARD_8;
		case 25:
			return KEYBOARD_9;
		case 29:
			return KEYBOARD_0;
		case 27:
			return KEYBOARD_HYPHEN;
		case 24:
			return KEYBOARD_EQUAL_SIGN;
		case 51:
			return KEYBOARD_DELETE_OR_BACKSPACE;
		case 48:
			return KEYBOARD_TAB;
		case 12:
			return KEYBOARD_Q;
		case 13:
			return KEYBOARD_W;
		case 14:
			return KEYBOARD_E;
		case 15:
			return KEYBOARD_R;
		case 17:
			return KEYBOARD_T;
		case 16:
			return KEYBOARD_Y;
		case 32:
			return KEYBOARD_U;
		case 34:
			return KEYBOARD_I;
		case 31:
			return KEYBOARD_O;
		case 35:
			return KEYBOARD_P;
		case 33:
			return KEYBOARD_OPEN_BRACKET;
		case 30:
			return KEYBOARD_CLOSE_BRACKET;
		case 42:
			return KEYBOARD_BACKSLASH;
		case 0:
			return KEYBOARD_A;
		case 1:
			return KEYBOARD_S;
		case 2:
			return KEYBOARD_D;
		case 3:
			return KEYBOARD_F;
		case 5:
			return KEYBOARD_G;
		case 4:
			return KEYBOARD_H;
		case 38:
			return KEYBOARD_J;
		case 40:
			return KEYBOARD_K;
		case 37:
			return KEYBOARD_L;
		case 41:
			return KEYBOARD_SEMICOLON;
		case 39:
			return KEYBOARD_QUOTE;
		case 36:
			return KEYBOARD_RETURN_OR_ENTER;
		case 6:
			return KEYBOARD_Z;
		case 7:
			return KEYBOARD_X;
		case 8:
			return KEYBOARD_C;
		case 9:
			return KEYBOARD_V;
		case 11:
			return KEYBOARD_B;
		case 45:
			return KEYBOARD_N;
		case 46:
			return KEYBOARD_M;
		case 43:
			return KEYBOARD_COMMA;
		case 47:
			return KEYBOARD_PERIOD;
		case 44:
			return KEYBOARD_SLASH;
		case 49:
			return KEYBOARD_SPACEBAR;
		case 123:
			return KEYBOARD_LEFT_ARROW;
		case 126:
			return KEYBOARD_UP_ARROW;
		case 125:
			return KEYBOARD_DOWN_ARROW;
		case 124:
			return KEYBOARD_RIGHT_ARROW;
		case 114:
			return KEYBOARD_INSERT;
		case 115:
			return KEYBOARD_HOME;
		case 116:
			return KEYBOARD_PAGE_UP;
		case 117:
			return KEYBOARD_DELETE_FORWARD;
		case 119:
			return KEYBOARD_END;
		case 121:
			return KEYBOARD_PAGE_DOWN;
		case 71:
			return KEYPAD_NUM_LOCK;
		case 81:
			return KEYPAD_EQUAL_SIGN;
		case 75:
			return KEYPAD_SLASH;
		case 67:
			return KEYPAD_ASTERISK;
		case 78:
			return KEYPAD_HYPHEN;
		case 69:
			return KEYPAD_PLUS;
		case 65:
			return KEYPAD_PERIOD;
		case 76:
			return KEYPAD_ENTER;
		case 83:
			return KEYPAD_1;
		case 84:
			return KEYPAD_2;
		case 85:
			return KEYPAD_3;
		case 86:
			return KEYPAD_4;
		case 87:
			return KEYPAD_5;
		case 88:
			return KEYPAD_6;
		case 89:
			return KEYPAD_7;
		case 91:
			return KEYPAD_8;
		case 92:
			return KEYPAD_9;
		case 82:
			return KEYPAD_0;
		default:
			return 0;
	}
}

static unsigned int NSEventKeyModifiersToShellKeyModifiers(NSUInteger modifiers) {
	unsigned int result = 0;
	
	if ((modifiers & NSShiftKeyMask)) {
		result |= MODIFIER_SHIFT_BIT;
	}
	if (modifiers & NSControlKeyMask) {
		result |= MODIFIER_CONTROL_BIT;
	}
	if (modifiers & NSAlternateKeyMask) {
		result |= MODIFIER_ALT_BIT;
	}
	if (modifiers & NSCommandKeyMask) {
		result |= MODIFIER_COMMAND_BIT;
	}
	if (modifiers & NSAlphaShiftKeyMask) {
		result |= MODIFIER_CAPS_LOCK_BIT;
	}
	
	return result;
}

- (void) keyDown: (NSEvent *) event {
	Target_keyDown([[event characters] characterAtIndex: 0], NSEventKeyCodeToShellKeyCode([event keyCode]), NSEventKeyModifiersToShellKeyModifiers([event modifierFlags]));
}

- (void) keyUp: (NSEvent *) event {
	Target_keyUp([[event characters] characterAtIndex: 0], NSEventKeyCodeToShellKeyCode([event keyCode]), NSEventKeyModifiersToShellKeyModifiers([event modifierFlags]));
}

- (void) flagsChanged: (NSEvent *) event {
	Target_keyModifiersChanged(NSEventKeyModifiersToShellKeyModifiers([event modifierFlags]));
}

- (BOOL) acceptsFirstResponder {
	return YES;
}

- (void) initCalled {
	initCalled = YES;
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
	
	if (!initCalled) {
		[self setNeedsDisplay: YES];
		return;
	}
	
	[[self openGLContext] makeCurrentContext];
	
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
	
	[[self openGLContext] flushBuffer];
}

- (void) drawTimer: (NSTimer *) timer {
	[self setNeedsDisplay: YES];
}

- (void) reshape {
	NSRect bounds;
	
	bounds = [self bounds];
	if (bounds.size.width != lastWidth || bounds.size.height != lastHeight) {
		lastWidth = bounds.size.width;
		lastHeight = bounds.size.height;
		glViewport(0, 0, lastWidth, lastHeight);
		Target_resized(lastWidth, lastHeight);
	}
}

- (BOOL) isFlipped {
	return YES;
}

@end
