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

#import "eaglshell/EAGLShellApplication.h"
#include "eaglshell/EAGLShellCallbacks.h"
#include "eaglshell/EAGLTarget.h"
#import "eaglshell/EAGLView.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellKeyCodes.h"

#ifndef __IPHONE_3_2
typedef enum {
    UIStatusBarAnimationNone,
    UIStatusBarAnimationFade,
    UIStatusBarAnimationSlide,
} UIStatusBarAnimation;
@interface UIApplication ()
- (void)setStatusBarHidden:(BOOL)hidden withAnimation:(UIStatusBarAnimation)animation;
@end
#endif

int g_argc = 0;
char ** g_argv = NULL;
bool g_mouseDeltaMode = false;

extern bool mainLoopCalled;

@implementation EAGLShellApplication

- (BOOL) application: (UIApplication *) application didFinishLaunchingWithOptions: (NSDictionary *) dictionary {
	[self setStatusBarOrientation: UIInterfaceOrientationPortrait animated: NO];
	if ([[[UIDevice currentDevice] systemVersion] compare: @"3.2" options: NSNumericSearch] != NSOrderedAscending) {
		[self setStatusBarHidden: YES withAnimation: UIStatusBarAnimationNone];
	} else {
		[self setStatusBarHidden: YES animated: NO];
	}
	self.applicationSupportsShakeToEdit = NO;
	
	if ([dictionary objectForKey: UIApplicationLaunchOptionsURLKey] != nil && openURLCallback != NULL) {
		openURLCallback([[[dictionary objectForKey: UIApplicationLaunchOptionsURLKey] absoluteString] UTF8String]);
	}
	
	window = [[UIWindow alloc] initWithFrame: [UIScreen mainScreen].bounds];
	view = [[EAGLView alloc] initWithFrame: [UIScreen mainScreen].applicationFrame];
	
	if (view == nil) {
		exit(EXIT_FAILURE);
	}
	
	textField = [[UITextField alloc] initWithFrame: CGRectMake(0, 0, 0, 0)];
	textField.text = @" ";
	textField.delegate = self;
	[view addSubview: textField];
	[window addSubview: view];
	[window makeKeyAndVisible];
	
	if (resizeCallback != NULL) {
		resizeCallback([UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height);
	}
	Target_init();
	
	if (!mainLoopCalled) {
		exit(EXIT_SUCCESS);
	}
	
	[view startAnimation];
	
	return YES;
}

- (void) applicationWillResignActive: (UIApplication *) application {
}

- (void) applicationDidBecomeActive: (UIApplication *) application {
}

- (void) applicationWillTerminate: (UIApplication *) application {
}

- (void) application: (UIApplication *) application didChangeStatusBarOrientation: (UIInterfaceOrientation) oldStatusBarOrientation {
	[self updateViewFrame];
}

- (void) application: (UIApplication *) application didChangeStatusBarFrame: (CGRect) oldStatusBarFrame {
	[self updateViewFrame];
}

- (void) dealloc {
	[textField release];
	[view release];
	[window release];
	[super dealloc];
}

- (enum EAGLShellOpenGLVersion) chosenOpenGLVersion {
	return [view chosenOpenGLVersion];
}

- (void) redisplayPosted {
	[view redisplayPosted];
}

- (void) showKeyboard {
	[textField becomeFirstResponder];
}

- (void) hideKeyboard {
	[textField resignFirstResponder];
}

- (BOOL) isKeyboardVisible {
	return [textField isFirstResponder];
}

- (void) updateViewFrame {
	view.frame = [UIScreen mainScreen].applicationFrame;
}

- (void) enableAccelerometerWithInterval: (NSTimeInterval) interval {
	[UIAccelerometer sharedAccelerometer].updateInterval = interval;
	[UIAccelerometer sharedAccelerometer].delegate = self;
}

- (void) disableAccelerometer {
	[UIAccelerometer sharedAccelerometer].delegate = nil;
}

- (void) accelerometer: (UIAccelerometer *) accelerometer didAccelerate: (UIAcceleration *) acceleration {
	if (accelerometerCallback != NULL) {
		accelerometerCallback(acceleration.x, acceleration.y, acceleration.z);
	}
}

static unsigned int unicharToShellKeyCode(unichar charCode) {
	switch (charCode) {
		case '\x1B':
			return KEYBOARD_ESCAPE;
		case '`':
		case '~':
			return KEYBOARD_GRAVE_ACCENT_AND_TILDE;
		case '1':
		case '!':
			return KEYBOARD_1;
		case '2':
		case '@':
			return KEYBOARD_2;
		case '3':
		case '#':
			return KEYBOARD_3;
		case '4':
		case '$':
			return KEYBOARD_4;
		case '5':
		case '%':
			return KEYBOARD_5;
		case '6':
		case '^':
			return KEYBOARD_6;
		case '7':
		case '&':
			return KEYBOARD_7;
		case '8':
		case '*':
			return KEYBOARD_8;
		case '9':
		case '(':
			return KEYBOARD_9;
		case '0':
		case ')':
			return KEYBOARD_0;
		case '-':
		case '_':
			return KEYBOARD_HYPHEN;
		case '=':
		case '+':
			return KEYBOARD_EQUAL_SIGN;
		case '\x7F':
			return KEYBOARD_DELETE_OR_BACKSPACE;
		case '\t':
			return KEYBOARD_TAB;
		case 'q':
		case 'Q':
			return KEYBOARD_Q;
		case 'w':
		case 'W':
			return KEYBOARD_W;
		case 'e':
		case 'E':
			return KEYBOARD_E;
		case 'r':
		case 'R':
			return KEYBOARD_R;
		case 't':
		case 'T':
			return KEYBOARD_T;
		case 'y':
		case 'Y':
			return KEYBOARD_Y;
		case 'u':
		case 'U':
			return KEYBOARD_U;
		case 'i':
		case 'I':
			return KEYBOARD_I;
		case 'o':
		case 'O':
			return KEYBOARD_O;
		case 'p':
		case 'P':
			return KEYBOARD_P;
		case '[':
		case '{':
			return KEYBOARD_OPEN_BRACKET;
		case ']':
		case '}':
			return KEYBOARD_CLOSE_BRACKET;
		case '\\':
		case '|':
			return KEYBOARD_BACKSLASH;
		case 'a':
		case 'A':
			return KEYBOARD_A;
		case 's':
		case 'S':
			return KEYBOARD_S;
		case 'd':
		case 'D':
			return KEYBOARD_D;
		case 'f':
		case 'F':
			return KEYBOARD_F;
		case 'g':
		case 'G':
			return KEYBOARD_G;
		case 'h':
		case 'H':
			return KEYBOARD_H;
		case 'j':
		case 'J':
			return KEYBOARD_J;
		case 'k':
		case 'K':
			return KEYBOARD_K;
		case 'l':
		case 'L':
			return KEYBOARD_L;
		case ';':
		case ':':
			return KEYBOARD_SEMICOLON;
		case '\'':
		case '"':
			return KEYBOARD_QUOTE;
		case '\r':
		case '\n':
			return KEYBOARD_RETURN_OR_ENTER;
		case 'z':
		case 'Z':
			return KEYBOARD_Z;
		case 'x':
		case 'X':
			return KEYBOARD_X;
		case 'c':
		case 'C':
			return KEYBOARD_C;
		case 'v':
		case 'V':
			return KEYBOARD_V;
		case 'b':
		case 'B':
			return KEYBOARD_B;
		case 'n':
		case 'N':
			return KEYBOARD_N;
		case 'm':
		case 'M':
			return KEYBOARD_M;
		case ',':
		case '<':
			return KEYBOARD_COMMA;
		case '.':
		case '>':
			return KEYBOARD_PERIOD;
		case '/':
		case '?':
			return KEYBOARD_SLASH;
		case ' ':
			return KEYBOARD_SPACEBAR;
		default:
			return 0;
	}
}

- (BOOL) textField: (UITextField *) textField shouldChangeCharactersInRange: (NSRange) range replacementString: (NSString *) string {
	unsigned int charIndex;
	
	if (range.length > 0 && [string length] == 0) {
		if (keyDownCallback != NULL) {
			keyDownCallback('\b', KEYBOARD_DELETE_OR_BACKSPACE, 0);
		}
		if (keyUpCallback != NULL) {
			keyUpCallback(KEYBOARD_DELETE_OR_BACKSPACE, 0);
		}
	} else {
		for (charIndex = 0; charIndex < [string length]; charIndex++) {
			if (keyDownCallback != NULL) {
				keyDownCallback([string characterAtIndex: charIndex], unicharToShellKeyCode([string characterAtIndex: charIndex]), 0);
			}
			if (keyUpCallback != NULL) {
				keyUpCallback(unicharToShellKeyCode([string characterAtIndex: charIndex]), 0);
			}
		}
	}
	
	return NO;
}

- (BOOL) textFieldShouldReturn: (UITextField *) textField {
	if (keyDownCallback != NULL) {
		keyDownCallback('\n', KEYBOARD_RETURN_OR_ENTER, 0);
	}
	if (keyUpCallback != NULL) {
		keyUpCallback(KEYBOARD_RETURN_OR_ENTER, 0);
	}
	return NO;
}

@end
