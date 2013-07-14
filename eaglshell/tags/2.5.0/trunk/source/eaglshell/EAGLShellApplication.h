/*
  Copyright (c) 2013 Alex Diener
  
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
#include "eaglshell/EAGLShell.h"

extern int g_argc;
extern char ** g_argv;
extern bool g_mouseDeltaMode;

@class EAGLView;

@interface EAGLShellApplication : UIApplication <UIApplicationDelegate, UITextFieldDelegate, UIAccelerometerDelegate> {
	UIWindow * window;
	EAGLView * view;
	UITextField * textField;
}

- (enum EAGLShellOpenGLVersion) chosenOpenGLVersion;

- (void) redisplayPosted;

- (void) showKeyboard;
- (void) hideKeyboard;
- (BOOL) isKeyboardVisible;

- (void) updateViewFrame;

- (void) enableAccelerometerWithInterval: (NSTimeInterval) interval;
- (void) disableAccelerometer;

@end
