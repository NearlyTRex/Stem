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

#import "nsopenglshell/NSOpenGLShellApplication.h"
#include "nsopenglshell/NSOpenGLShellCallbacks.h"
#import "nsopenglshell/NSOpenGLShellView.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#include "shell/Shell.h"
#include "shell/ShellCallbacks.h"

int g_argc = 0;
const char ** g_argv = NULL;
struct NSOpenGLShellConfiguration g_configuration;
bool g_mouseDeltaMode = false;

extern bool mainLoopCalled;

@interface NSWindowWithSquareCorners : NSWindow
@end
@implementation NSWindowWithSquareCorners
- (BOOL) bottomCornerRounded {
	return NO;
}
@end

@implementation NSOpenGLShellApplication

static NSScreen * screenForRect(NSRect rect) {
	NSScreen * screen;
	NSRect frame, intersection;
	unsigned int screenIndex, bestScreenIndex = 0;
	CGFloat coverage, bestCoverage = 0.0;
	
	for (screenIndex = 0; screenIndex < [[NSScreen screens] count]; screenIndex++) {
		screen = [[NSScreen screens] objectAtIndex: screenIndex];
		frame = [screen frame];
		frame.origin.y = [[NSScreen mainScreen] frame].size.height - frame.origin.y - frame.size.height;
		intersection = NSIntersectionRect(rect, frame);
		coverage = intersection.size.width * intersection.size.height;
		if (coverage > bestCoverage) {
			bestCoverage = coverage;
			bestScreenIndex = screenIndex;
		}
	}
	return [[NSScreen screens] objectAtIndex: bestScreenIndex];
}

- (void) applicationDidFinishLaunching: (NSNotification *) notification {
	NSScreen * screen;
	NSRect contentRect;
	
	contentRect = NSMakeRect(g_configuration.windowX, [[NSScreen mainScreen] frame].size.height - g_configuration.windowY - g_configuration.windowHeight, g_configuration.windowWidth, g_configuration.windowHeight);
	screen = screenForRect(contentRect);
	contentRect.origin.x -= [screen frame].origin.x;
	contentRect.origin.y -= [screen frame].origin.y;
	window = [[NSWindowWithSquareCorners alloc] initWithContentRect: contentRect
	                                                      styleMask: NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
	                                                        backing: NSBackingStoreBuffered
	                                                          defer: NO
	                                                         screen: screen];
	[window setDelegate: self];
	[window setTitle: [NSString stringWithUTF8String: g_configuration.windowTitle]];
	[window setAcceptsMouseMovedEvents: YES];
	[window setContentMinSize: NSMakeSize(64, 32)];
	view = [[NSOpenGLShellView alloc] initWithFrame: NSMakeRect(0, 0, g_configuration.windowWidth, g_configuration.windowHeight) configuration: g_configuration];
	
	if (view == nil) {
		exit(EXIT_FAILURE);
	}
	
	[window setContentView: view];
	[window setInitialFirstResponder: view];
	
	if (resizeCallback != NULL) {
		NSRect viewBounds;
		
		viewBounds = [view bounds];
		if ([view respondsToSelector: @selector(convertRectToBacking:)]) {
			viewBounds = [view convertRectToBacking: viewBounds];
		}
		resizeCallback(viewBounds.size.width, viewBounds.size.height);
	}
	Target_init();
	[view initCalled];
	[view displayIfNeeded];
	
	[window orderFront: nil];
	[[view window] makeKeyAndOrderFront: nil];
	
	if (!mainLoopCalled) {
		exit(EXIT_SUCCESS);
	}
}

- (void) applicationWillResignActive: (NSNotification *) notification {
	if (![window isMiniaturized] && backgroundedCallback != NULL) {
		backgroundedCallback();
	}
}

- (void) applicationDidBecomeActive: (NSNotification *) notification {
	if (![window isMiniaturized] && foregroundedCallback != NULL) {
		foregroundedCallback();
	}
}

- (void) dealloc {
	[view release];
	[window release];
	[super dealloc];
}

- (void) sendEvent: (NSEvent *) event {
	if ([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask)) {
		[[self keyWindow] sendEvent: event];
	} else {
		[super sendEvent: event];
	}
}

- (void) hide: (id) sender {
	if (![view isInFullScreenMode]) {
		[super hide: sender];
	}
}

- (void) toggleFullScreen: (id) sender {
	if (Shell_isFullScreen()) {
		Shell_exitFullScreen();
	} else {
		Shell_enterFullScreen(Shell_getDisplayIndexFromWindow());
	}
}

- (NSWindow *) window {
	return window;
}

- (NSOpenGLShellView *) view {
	return view;
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) application {
  return YES;
}

- (NSApplicationTerminateReply) applicationShouldTerminate: (NSNotification *) notification {
	if (confirmQuitCallback == NULL) {
		return true;
	}
	return confirmQuitCallback();
}

- (BOOL) windowShouldClose: (NSNotification *) notification {
	if (confirmQuitCallback == NULL) {
		return true;
	}
	return confirmQuitCallback();
}

- (void) windowWillClose: (NSNotification *) notification {
	exit(EXIT_SUCCESS);
}

- (void) windowWillMiniaturize: (NSNotification *) notification {
	if ([self isActive] && backgroundedCallback != NULL) {
		backgroundedCallback();
	}
}

- (void) windowDidDeminiaturize: (NSNotification *) notification {
	if ([self isActive] && foregroundedCallback != NULL) {
		foregroundedCallback();
	}
}

@end
