/*
  Copyright (c) 2012 Alex Diener
  
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
#include "shell/Target.h"

int g_argc = 0;
const char ** g_argv = NULL;
struct NSOpenGLShellConfiguration g_configuration;

extern bool mainLoopCalled;

@implementation NSOpenGLShellApplication

- (void) applicationDidFinishLaunching: (NSNotification *) notification {
	window = [[NSWindow alloc] initWithContentRect: NSMakeRect(g_configuration.windowX, [[NSScreen mainScreen] frame].size.height - g_configuration.windowY, g_configuration.windowWidth, g_configuration.windowHeight)
	                                     styleMask: NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
	                                       backing: NSBackingStoreBuffered
	                                         defer: NO];
	[window setDelegate: self];
	[window setTitle: [NSString stringWithUTF8String: g_configuration.windowTitle]];
	[window setAcceptsMouseMovedEvents: YES];
	view = [[NSOpenGLShellView alloc] initWithFrame: NSMakeRect(0, 0, g_configuration.windowWidth, g_configuration.windowHeight) configuration: g_configuration];
	
	if (view == nil) {
		exit(EXIT_FAILURE);
	}
	
	[window setContentView: view];
	[window setInitialFirstResponder: view];
	
	Target_resized([view bounds].size.width, [view bounds].size.height);
	Target_init();
	[view initCalled];
	[view displayIfNeeded];
	
	[window orderFront: nil];
	[[view window] makeKeyAndOrderFront: nil];
	
	if (!mainLoopCalled) {
		exit(EXIT_SUCCESS);
	}
	
	[view startAnimation];
}

- (void) applicationWillResignActive: (NSNotification *) notification {
	if (![window isMiniaturized]) {
		Target_backgrounded();
	}
}

- (void) applicationDidBecomeActive: (NSNotification *) notification {
	if (![window isMiniaturized]) {
		Target_foregrounded();
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

- (void) redisplayPosted {
	[view redisplayPosted];
}

- (bool) isFullScreen {
	return [view isInFullScreenMode];
}

- (void) toggleFullScreen {
	[view toggleFullScreen];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication *) application {
  return YES;
}

- (void) windowWillMiniaturize: (NSNotification *) notification {
	if ([self isActive]) {
		Target_backgrounded();
	}
}

- (void) windowDidDeminiaturize: (NSNotification *) notification {
	if ([self isActive]) {
		Target_foregrounded();
	}
}

@end
