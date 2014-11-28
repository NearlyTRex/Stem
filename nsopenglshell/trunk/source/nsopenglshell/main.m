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

#import <Cocoa/Cocoa.h>
#import "nsopenglshell/NSOpenGLShellApplication.h"
#include "nsopenglshell/NSOpenGLTarget.h"
#include "shell/Shell.h"

@interface NSApplication (NSAppleMenu)
- (void)setAppleMenu:(NSMenu*)m;
@end

int main(int argc, const char ** argv) {
	NSAutoreleasePool * pool;
	NSString * applicationName;
	NSMenu * menuBar, * applicationMenu, * windowMenu;
	NSMenuItem * applicationMenuItem, * windowMenuItem;
	
	pool = [[NSAutoreleasePool alloc] init];
	
	g_argc = argc;
	g_argv = argv;
	g_configuration.windowX = 2;
	g_configuration.windowY = 28;
	g_configuration.windowWidth = 800;
	g_configuration.windowHeight = 600;
	g_configuration.windowTitle = "Change configuration->windowTitle in NSOpenGLTarget_configure()!";
	g_configuration.fullScreenMenuItem = false;
	g_configuration.displayMode.doubleBuffer = true;
	g_configuration.displayMode.depthBuffer = false;
	g_configuration.displayMode.depthBits = 32;
	g_configuration.displayMode.stencilBuffer = false;
	g_configuration.displayMode.stencilBits = 8;
	g_configuration.displayMode.accumBuffer = false;
	g_configuration.displayMode.accumBits = 32;
	g_configuration.displayMode.multisample = false;
	g_configuration.displayMode.sampleBuffers = 1;
	g_configuration.displayMode.samples = 4;
	
	[[NSOpenGLShellApplication sharedApplication] setDelegate: (NSOpenGLShellApplication *) [NSOpenGLShellApplication sharedApplication]];
	
	NSOpenGLTarget_configure(g_argc, g_argv, &g_configuration);
	
	applicationName = [[[NSBundle mainBundle] infoDictionary] objectForKey: @"CFBundleName"];
	if (applicationName == nil) {
		applicationName = @"[Set CFBundleName in Info.plist to change this!]";
	}
	
	// Special thanks to GLFW, SDL, and Keith Bauer for nibless menu setup code
	menuBar = [[NSMenu alloc] init];
	[NSApp setMainMenu: menuBar];
	
	applicationMenuItem = [menuBar addItemWithTitle: @"" action: NULL keyEquivalent: @""];
	applicationMenu = [[NSMenu alloc] init];
	[applicationMenuItem setSubmenu: applicationMenu];
	[applicationMenu addItemWithTitle: [NSString stringWithFormat:@"Hide %@", applicationName] action: @selector(hide:) keyEquivalent: @"h"];
	[[applicationMenu addItemWithTitle: @"Hide Others" action: @selector(hideOtherApplications:) keyEquivalent: @"h"] setKeyEquivalentModifierMask: NSAlternateKeyMask | NSCommandKeyMask];
	[applicationMenu addItemWithTitle: @"Show All" action: @selector(unhideAllApplications:) keyEquivalent: @""];
	[applicationMenu addItem: [NSMenuItem separatorItem]];
	[applicationMenu addItemWithTitle: [NSString stringWithFormat: @"Quit %@", applicationName] action: @selector(terminate:) keyEquivalent: @"q"];
	
	windowMenuItem = [menuBar addItemWithTitle: @"" action: NULL keyEquivalent: @""];
	windowMenu = [[NSMenu alloc] initWithTitle: @"Window"];
	[windowMenuItem setSubmenu: windowMenu];
	[windowMenu addItemWithTitle: @"Close" action: @selector(performClose:) keyEquivalent: @"w"];
	[windowMenu addItemWithTitle: @"Miniaturize" action: @selector(performMiniaturize:) keyEquivalent: @"m"];
	[windowMenu addItemWithTitle: @"Zoom" action: @selector(performZoom:) keyEquivalent:@""];
	[windowMenu addItem: [NSMenuItem separatorItem]];
	//[windowMenu addItemWithTitle: @"Bring All to Front" action: @selector(arrangeInFront:) keyEquivalent:@""];
	if (g_configuration.fullScreenMenuItem) {
		NSMenuItem * menuItem = [windowMenu addItemWithTitle: @"Enter Full Screen" action: @selector(toggleFullScreen:) keyEquivalent: @"f"];
		[menuItem setKeyEquivalentModifierMask: NSControlKeyMask | NSCommandKeyMask];
		[menuItem setTarget: NSApp];
	}
	[NSApp setWindowsMenu: windowMenu];
	
	if ([NSApp respondsToSelector: @selector(setAppleMenu:)]) {
		[NSApp setAppleMenu: applicationMenu];
	} else {
		fprintf(stderr, "WARNING: NSApplication doesn't respond to -setAppleMenu:. The application menu may not work correctly.\n");
	}
	
	[NSApp activateIgnoringOtherApps: YES];
	[NSApp run];
	[pool release];
}
