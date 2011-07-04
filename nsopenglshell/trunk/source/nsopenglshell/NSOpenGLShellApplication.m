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

#import "nsopenglshell/NSOpenGLShellApplication.h"
#import "nsopenglshell/NSOpenGLShellView.h"
#include "shell/ShellKeyCodes.h"
#include "shell/Target.h"
#include "nsopenglshell/NSOpenGLTarget.h"

int g_argc = 0;
const char ** g_argv = NULL;

extern bool mainLoopCalled;

@implementation NSOpenGLShellApplication

- (void) applicationDidFinishLaunching: (NSNotification *) notification {
	struct NSOpenGLShellConfiguration configuration;
	
	configuration.windowX = 2;
	configuration.windowY = 28;
	configuration.windowWidth = 800;
	configuration.windowHeight = 600;
	configuration.windowTitle = "Change configuration->windowTitle in NSOpenGLTarget_configure()!";
	configuration.displayMode.doubleBuffer = true;
	configuration.displayMode.depthBuffer = false;
	configuration.displayMode.stencilBuffer = false;
	configuration.displayMode.accumBuffer = false;
	configuration.displayMode.multisample = false;
	
	NSOpenGLTarget_configure(g_argc, g_argv, &configuration);
	
	window = [[NSWindow alloc] initWithContentRect: NSMakeRect(configuration.windowX, [[NSScreen mainScreen] frame].size.height - configuration.windowY, configuration.windowWidth, configuration.windowHeight)
	                                     styleMask: NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
	                                       backing: NSBackingStoreBuffered
	                                         defer: NO];
	[window setTitle: [NSString stringWithUTF8String: configuration.windowTitle]];
	[window setAcceptsMouseMovedEvents: YES];
	[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(windowWillClose:) name: NSWindowWillCloseNotification object: window];
	view = [[NSOpenGLShellView alloc] initWithFrame: NSMakeRect(0, 0, configuration.windowWidth, configuration.windowHeight) configuration: configuration];
	
	if (view == nil) {
		exit(EXIT_FAILURE);
	}
	
	[window setContentView: view];
	[window makeKeyAndOrderFront: nil];
	
	Target_resized([view bounds].size.width, [view bounds].size.height);
	Target_init();
	
	if (!mainLoopCalled) {
		exit(EXIT_SUCCESS);
	}
	
	[view startAnimation];
}

- (void) applicationWillResignActive: (NSNotification *) notification {
	// TODO
	//NSOpenGLTarget_backgrounded();
}

- (void) applicationDidBecomeActive: (NSNotification *) notification {
	// TODO
	//NSOpenGLTarget_foregrounded();
}

- (void) dealloc {
	[view release];
	[window release];
	[super dealloc];
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

- (void) sendEvent: (NSEvent *) event {
	if (![view isFullScreen] && [self keyWindow] != [self mainWindow]) {
		[super sendEvent: event];
		return;
	}
	
	switch ([event type]) {
		case NSLeftMouseDown:
		case NSRightMouseDown:
		case NSOtherMouseDown:
			if ([view isFullScreen]) {
				Target_mouseDown([event buttonNumber], [event locationInWindow].x, [[NSScreen mainScreen] frame].size.height - [event locationInWindow].y);
			}
			[super sendEvent: event];
			break;
		case NSLeftMouseUp:
		case NSRightMouseUp:
		case NSOtherMouseUp:
			if ([view isFullScreen]) {
				Target_mouseUp([event buttonNumber], [event locationInWindow].x, [[NSScreen mainScreen] frame].size.height - [event locationInWindow].y);
			}
			[super sendEvent: event];
			break;
		case NSMouseMoved:
			if ([view isFullScreen]) {
				Target_mouseMoved([event locationInWindow].x, [[NSScreen mainScreen] frame].size.height - [event locationInWindow].y);
			} else {
				Target_mouseMoved([event locationInWindow].x, [view bounds].size.height - [event locationInWindow].y);
			}
			[super sendEvent: event];
			break;
		case NSLeftMouseDragged:
			// TODO: [event buttonNumber] doesn't behave correctly for the Target_mouseDragged API; need to track bitfield with every button number sent to Target_mouseDown (collaboratively with NSOpenGLShellView)
			if ([view isFullScreen]) {
				Target_mouseDragged([event buttonNumber], [event locationInWindow].x, [[NSScreen mainScreen] frame].size.height - [event locationInWindow].y);
			} else {
				Target_mouseDragged([event buttonNumber], [event locationInWindow].x, [view bounds].size.height - [event locationInWindow].y);
			}
			[super sendEvent: event];
			break;
		case NSKeyDown:
			if ([event modifierFlags] & NSCommandKeyMask) {
				[super sendEvent: event];
				break;
			}
			Target_keyDown([[event characters] characterAtIndex: 0], NSEventKeyCodeToShellKeyCode([event keyCode]));
			break;
		case NSKeyUp:
			Target_keyUp([[event characters] characterAtIndex: 0], NSEventKeyCodeToShellKeyCode([event keyCode]));
			break;
		default:
			[super sendEvent: event];
			break;
	}
}

- (void) hide: (id) sender {
	if (![view isFullScreen]) {
		[super hide: sender];
	}
}

- (void) redisplayPosted {
	[view redisplayPosted];
}

- (bool) isFullScreen {
	return [view isFullScreen];
}

- (void) toggleFullScreen {
	[view toggleFullScreen];
}

- (void) windowWillClose: (NSNotification *) notification {
	exit(EXIT_SUCCESS);
}

@end
