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

#import "nsopenglshell/NSOpenGLShellView.h"
#include "nsopenglshell/NSOpenGLShellCallbacks.h"
#import "nsopenglshell/NSOpenGLShellApplication.h"
#include "shell/Shell.h"
#include "shell/ShellKeyCodes.h"
#include <OpenGL/glu.h>

@interface NSView ()
- (void)setWantsBestResolutionOpenGLSurface:(BOOL)flag;
- (NSRect)convertRectToBacking:(NSRect)aRect;
- (NSPoint)convertPointToBacking:(NSPoint)aPoint;
@end

@implementation NSOpenGLShellView

- (void) setVSync: (BOOL) sync {
	GLint syncGL = sync;
	[[self openGLContext] setValues: &syncGL forParameter: NSOpenGLCPSwapInterval];
}

- (id) initWithFrame: (NSRect) frame configuration: (struct NSOpenGLShellConfiguration) configuration {
	NSOpenGLPixelFormat * pixelFormat;
	NSOpenGLPixelFormatAttribute attributes[15];
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
	if (configuration.useGLCoreProfile) {
		attributes[numAttributes++] = NSOpenGLPFAOpenGLProfile;
		attributes[numAttributes++] = NSOpenGLProfileVersion3_2Core;
	}
	attributes[numAttributes++] = NSOpenGLPFAAccelerated;
	attributes[numAttributes++] = NSOpenGLPFANoRecovery;
	attributes[numAttributes] = 0;
	pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: attributes];
	if ((self = [super initWithFrame: frame pixelFormat: pixelFormat]) != nil) {
		if ([self respondsToSelector: @selector(setWantsBestResolutionOpenGLSurface:)]) {
			[self setWantsBestResolutionOpenGLSurface: YES];
		}
		[[self openGLContext] makeCurrentContext];
		
		vsyncWindow = VSYNC_DEFAULT_WINDOW;
		vsyncFullscreen = VSYNC_DEFAULT_FULLSCREEN;
		[self setVSync: vsyncWindow];
		
		[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(frameDidChange:) name: NSViewFrameDidChangeNotification object: nil];
	}
	[pixelFormat release];
	return self;
}

- (void) dealloc {
	[[NSNotificationCenter defaultCenter] removeObserver: self];
	[drawTimer invalidate];
	drawTimer = nil;
	[super dealloc];
}

- (bool) enterFullScreen: (NSScreen *) screen {
	BOOL success = YES;
	
	if (![self isInFullScreenMode]) {
		success = [self enterFullScreenMode: screen withOptions: [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool: NO], NSFullScreenModeAllScreens, nil]];
		if (success) {
			[self setVSync: vsyncFullscreen];
		}
		[[self window] makeFirstResponder: self];
	}
	return success;
}

- (void) exitFullScreen {
	if ([self isInFullScreenMode]) {
		[self exitFullScreenModeWithOptions: [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool: NO], NSFullScreenModeAllScreens, nil]];
		[self setVSync: vsyncWindow];
		[[self window] makeFirstResponder: self];
	}
}

- (void) setVSync: (BOOL) sync forFullscreen: (BOOL) fullscreen {
	if (fullscreen) {
		vsyncFullscreen = sync;
		if ([self isInFullScreenMode]) {
			[self setVSync: sync];
		}
		
	} else {
		vsyncWindow = sync;
		if (![self isInFullScreenMode]) {
			[self setVSync: sync];
		}
	}
}

- (NSPoint) pixelLocationForEvent: (NSEvent *) event {
	NSRect bounds;
	NSPoint location = [event locationInWindow];
	
	if ([self respondsToSelector: @selector(convertPointToBacking:)]) {
		location = [self convertPointToBacking: location];
		location.y = -location.y;
	}
	
	bounds = [self bounds];
	if ([self respondsToSelector: @selector(convertRectToBacking:)]) {
		bounds = [self convertRectToBacking: bounds];
	}
	location.y = bounds.size.height - location.y;
	
	return location;
}

- (void) mouseMoved: (NSEvent *) event {
	if (mouseMovedCallback != NULL) {
		if (g_mouseDeltaMode) {
			mouseMovedCallback([event deltaX], [event deltaY]);
		} else {
			NSPoint location = [self pixelLocationForEvent: event];
			mouseMovedCallback(location.x, location.y);
		}
	}
}

- (void) mouseDown: (NSEvent *) event {
	buttonMask |= 1 << [event buttonNumber];
	if (mouseDownCallback != NULL) {
		NSPoint location = [self pixelLocationForEvent: event];
		mouseDownCallback([event buttonNumber], location.x, location.y);
	}
}

- (void) mouseDragged: (NSEvent *) event {
	if (mouseDraggedCallback != NULL) {
		if (g_mouseDeltaMode) {
			mouseDraggedCallback(buttonMask, [event deltaX], [event deltaY]);
		} else {
			NSPoint location = [self pixelLocationForEvent: event];
			mouseDraggedCallback(buttonMask, location.x, location.y);
		}
	}
}

- (void) mouseUp: (NSEvent *) event {
	buttonMask &= ~(1 << [event buttonNumber]);
	if (mouseUpCallback != NULL) {
		NSPoint location = [self pixelLocationForEvent: event];
		mouseUpCallback([event buttonNumber], location.x, location.y);
	}
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

#define COLUMN_WIDTH 20
#define ROW_HEIGHT 20

- (void) scrollWheel: (NSEvent *) event {
	int deltaX = 0, deltaY = 0;
	
	if ([event phase] == NSEventPhaseNone && [event momentumPhase] == NSEventPhaseNone) {
		if ([event scrollingDeltaX] > 0) {
			deltaX = -1;
		} else if ([event scrollingDeltaX] < 0) {
			deltaX = 1;
		}
		if ([event scrollingDeltaY] > 0) {
			deltaY = -1;
		} else if ([event scrollingDeltaY] < 0) {
			deltaY = 1;
		}
		
	} else {
		if ([event phase] == NSEventPhaseBegan || [event phase] == NSEventPhaseEnded) {
			coalescedScrollDeltaX = coalescedScrollDeltaY = 0.0;
		}
		
		if ([event phase] >= NSEventPhaseBegan && [event phase] <= NSEventPhaseEnded && [event momentumPhase] == NSEventPhaseNone) {
			CGFloat fdeltaX = [event scrollingDeltaX];
			CGFloat fdeltaY = [event scrollingDeltaY];
			if (![event hasPreciseScrollingDeltas]) {
				fdeltaX *= COLUMN_WIDTH;
				fdeltaY *= ROW_HEIGHT;
			}
			coalescedScrollDeltaX -= fdeltaX;
			coalescedScrollDeltaY -= fdeltaY;
			
			deltaX = coalescedScrollDeltaX / ROW_HEIGHT;
			if (deltaX != 0) {
				coalescedScrollDeltaX = coalescedScrollDeltaX - deltaX * COLUMN_WIDTH;
			}
			
			deltaY = coalescedScrollDeltaY / ROW_HEIGHT;
			if (deltaY != 0) {
				coalescedScrollDeltaY = coalescedScrollDeltaY - deltaY * ROW_HEIGHT;
			}
		}
	}
	
	if (scrollWheelCallback != NULL && (deltaX != 0 || deltaY != 0)) {
		scrollWheelCallback(deltaX, deltaY);
	}
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
	if (keyDownCallback != NULL) {
		NSString * characters;
		
		characters = [event characters];
		keyDownCallback([characters length] > 0 ? [characters characterAtIndex: 0] : 0, NSEventKeyCodeToShellKeyCode([event keyCode]), NSEventKeyModifiersToShellKeyModifiers([event modifierFlags]), [event isARepeat]);
	}
}

- (void) keyUp: (NSEvent *) event {
	if (keyUpCallback != NULL) {
		keyUpCallback(NSEventKeyCodeToShellKeyCode([event keyCode]), NSEventKeyModifiersToShellKeyModifiers([event modifierFlags]));
	}
}

- (void) flagsChanged: (NSEvent *) event {
	unsigned int newModifierMask;
	
	newModifierMask = NSEventKeyModifiersToShellKeyModifiers([event modifierFlags]);
	if (newModifierMask != modifierMask) {
		if ((newModifierMask & MODIFIER_SHIFT_BIT) && !(modifierMask & MODIFIER_SHIFT_BIT)) {
			if (keyDownCallback != NULL) {
				keyDownCallback(0, KEYBOARD_LEFT_SHIFT, modifierMask, false);
			}
		} else if (!(newModifierMask & MODIFIER_SHIFT_BIT) && (modifierMask & MODIFIER_SHIFT_BIT)) {
			if (keyUpCallback != NULL) {
				keyUpCallback(KEYBOARD_LEFT_SHIFT, modifierMask);
			}
		}
		
		if ((newModifierMask & MODIFIER_CONTROL_BIT) && !(modifierMask & MODIFIER_CONTROL_BIT)) {
			if (keyDownCallback != NULL) {
				keyDownCallback(0, KEYBOARD_LEFT_CONTROL, modifierMask, false);
			}
		} else if (!(newModifierMask & MODIFIER_CONTROL_BIT) && (modifierMask & MODIFIER_CONTROL_BIT)) {
			if (keyUpCallback != NULL) {
				keyUpCallback(KEYBOARD_LEFT_CONTROL, modifierMask);
			}
		}
		
		if ((newModifierMask & MODIFIER_ALT_BIT) && !(modifierMask & MODIFIER_ALT_BIT)) {
			if (keyDownCallback != NULL) {
				keyDownCallback(0, KEYBOARD_LEFT_ALT, modifierMask, false);
			}
		} else if (!(newModifierMask & MODIFIER_ALT_BIT) && (modifierMask & MODIFIER_ALT_BIT)) {
			if (keyUpCallback != NULL) {
				keyUpCallback(KEYBOARD_LEFT_ALT, modifierMask);
			}
		}
		
		if ((newModifierMask & MODIFIER_COMMAND_BIT) && !(modifierMask & MODIFIER_COMMAND_BIT)) {
			if (keyDownCallback != NULL) {
				keyDownCallback(0, KEYBOARD_LEFT_GUI, modifierMask, false);
			}
		} else if (!(newModifierMask & MODIFIER_COMMAND_BIT) && (modifierMask & MODIFIER_COMMAND_BIT)) {
			if (keyUpCallback != NULL) {
				keyUpCallback(KEYBOARD_LEFT_GUI, modifierMask);
			}
		}
		
		if ((newModifierMask & MODIFIER_CAPS_LOCK_BIT) && !(modifierMask & MODIFIER_CAPS_LOCK_BIT)) {
			if (keyDownCallback != NULL) {
				keyDownCallback(0, KEYBOARD_CAPS_LOCK, modifierMask, false);
			}
		} else if (!(newModifierMask & MODIFIER_CAPS_LOCK_BIT) && (modifierMask & MODIFIER_CAPS_LOCK_BIT)) {
			if (keyUpCallback != NULL) {
				keyUpCallback(KEYBOARD_CAPS_LOCK, modifierMask);
			}
		}
		
		modifierMask = newModifierMask;
		if (keyModifiersChangedCallback != NULL) {
			keyModifiersChangedCallback(modifierMask);
		}
	}
}

- (BOOL) acceptsFirstResponder {
	return YES;
}

- (void) initCalled {
	initCalled = YES;
}

- (void) redisplay {
	if (drawTimer == nil) {
		drawTimer = [NSTimer timerWithTimeInterval: 1.0 / 240.0 target: self selector: @selector(drawTimer:) userInfo: nil repeats: YES];
		[[NSRunLoop mainRunLoop] addTimer: drawTimer forMode: NSRunLoopCommonModes];
	}
	redisplayWasPosted = YES;
}

- (bool) redisplayWasPosted {
	return redisplayWasPosted;
}

- (void) drawTimer: (NSTimer *) timer {
	[self draw];
}

- (void) drawRect: (NSRect) rect {
	if (!initCalled) {
		return;
	}
	[self draw];
}

#ifdef DEBUG
double NSOpenGLShell_lastFlushBufferTime;
#endif

- (void) draw {
#ifdef DEBUG
	GLenum error;
#endif
	
	redisplayWasPosted = NO;
	
	[[self openGLContext] makeCurrentContext];
	
	if (drawCallback != NULL && drawCallback()) {
#ifdef DEBUG
		double flushBufferTime = Shell_getCurrentTime();
#endif
		[[self openGLContext] flushBuffer];
#ifdef DEBUG
		NSOpenGLShell_lastFlushBufferTime = Shell_getCurrentTime() - flushBufferTime;
#endif
	}
	
#ifdef DEBUG
	error = glGetError();
	while (error != GL_NO_ERROR) {
		fprintf(stderr, "GL error: 0x%X\n", error);
		error = glGetError();
	}
#endif
	
	if (!redisplayWasPosted) {
		[drawTimer invalidate];
		drawTimer = nil;
	}
}

- (void) reshape {
	NSRect bounds;
	
	bounds = [self bounds];
	if ([self respondsToSelector: @selector(convertRectToBacking:)]) {
		bounds = [self convertRectToBacking: bounds];
	}
	if (bounds.size.width != lastWidth || bounds.size.height != lastHeight) {
		lastWidth = bounds.size.width;
		lastHeight = bounds.size.height;
		glViewport(0, 0, lastWidth, lastHeight);
		if (resizeCallback != NULL) {
			resizeCallback(lastWidth, lastHeight);
		}
	}
}

- (void) frameDidChange: (NSNotification *) notification {
	[[self openGLContext] update];
}

- (BOOL) isFlipped {
	return YES;
}

- (BOOL) isOpaque {
	return YES;
}

@end
