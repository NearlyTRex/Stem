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

#include "nsopenglshell/NSOpenGLShell.h"

#import <Cocoa/Cocoa.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#import <IOKit/ps/IOPowerSources.h>
#import <IOKit/ps/IOPSKeys.h>

#import "nsopenglshell/NSOpenGLShellApplication.h"
#include "shell/Shell.h"
#include "shell/ShellBatteryInfo.h"

bool mainLoopCalled = false;
static bool cursorHiddenByHide = false;

void Shell_mainLoop() {
	mainLoopCalled = true;
}

void Shell_redisplay() {
	[(NSOpenGLShellApplication *) [NSApplication sharedApplication] redisplayPosted];
}

bool Shell_isFullScreen() {
	return [(NSOpenGLShellApplication *) [NSApplication sharedApplication] isFullScreen];
}

bool Shell_setFullScreen(bool fullScreen) {
	if (fullScreen != Shell_isFullScreen()) {
		[(NSOpenGLShellApplication *) [NSApplication sharedApplication] toggleFullScreen];
	}
	return true;
}

double Shell_getCurrentTime() {
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * (double) timebaseInfo.numer / timebaseInfo.denom * 0.000000001;
}

const char * Shell_getResourcePath() {
	static char resourcePath[PATH_MAX];
	CFBundleRef mainBundle;
	CFURLRef resourcesDirectoryURL;
	
	mainBundle = CFBundleGetMainBundle();
	resourcesDirectoryURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	CFURLGetFileSystemRepresentation(resourcesDirectoryURL, true, (UInt8 *) resourcePath, PATH_MAX);
	CFRelease(resourcesDirectoryURL);
	
	return resourcePath;
}

enum ShellBatteryState Shell_getBatteryState() {
	CFTypeRef info;
	CFArrayRef list;
	CFDictionaryRef battery;
	enum ShellBatteryState batteryState = ShellBatteryState_notBatteryPowered;
	
	info = IOPSCopyPowerSourcesInfo();
	if (info == NULL) {
		return batteryState;
	}
	list = IOPSCopyPowerSourcesList(info);
	if (list == NULL) {
		CFRelease(info);
		return batteryState;
	}
	
	if (CFArrayGetCount(list) > 0 && (battery = IOPSGetPowerSourceDescription(info, CFArrayGetValueAtIndex(list, 0))) != NULL) {
		if (![[(NSDictionary *) battery objectForKey: @kIOPSIsPresentKey] boolValue]) {
			batteryState = ShellBatteryState_batteryMissing; // How to tell it shouldn't be ShellBatteryState_notBatteryPowered?
		} else if ([[(NSDictionary *) battery objectForKey: @kIOPSIsChargingKey] boolValue]) {
			batteryState = ShellBatteryState_charging;
		} else if ([(NSString *) [(NSDictionary *) battery objectForKey: @kIOPSPowerSourceStateKey] isEqualToString: @kIOPSACPowerValue]) {
			batteryState = ShellBatteryState_full;
		} else {
			batteryState = ShellBatteryState_unplugged;
		}
	}
	
	CFRelease(list);
	CFRelease(info);
	
	return batteryState;
}

float Shell_getBatteryLevel() {
	CFTypeRef info;
	CFArrayRef list;
	CFDictionaryRef battery;
	float batteryLevel = -1.0f;
	
	info = IOPSCopyPowerSourcesInfo();
	if (info == NULL) {
		return -1.0f;
	}
	list = IOPSCopyPowerSourcesList(info);
	if (list == NULL) {
		CFRelease(info);
		return -1.0f;
	}
	
	if (CFArrayGetCount(list) > 0 && (battery = IOPSGetPowerSourceDescription(info, CFArrayGetValueAtIndex(list, 0))) != NULL) {
		if ([[(NSDictionary *) battery objectForKey: @kIOPSIsPresentKey] boolValue]) {
			batteryLevel = [[(NSDictionary *) battery objectForKey: @kIOPSCurrentCapacityKey] floatValue] / [[(NSDictionary *) battery objectForKey: @kIOPSMaxCapacityKey] floatValue];
		}
	}
	
	CFRelease(list);
	CFRelease(info);
	
	return batteryLevel;
}

void NSOpenGLShell_setCursorVisible(bool visible) {
	if (visible) {
		if (cursorHiddenByHide) {
			cursorHiddenByHide = false;
			[NSCursor unhide];
		}
	} else {
		if (!cursorHiddenByHide) {
			cursorHiddenByHide = true;
			[NSCursor hide];
		}
	}
}

void NSOpenGLShell_hideCursorUntilMouseMoves() {
	[NSCursor setHiddenUntilMouseMoves: YES];
}

@interface NSCursor (MacOSX10_6Cursors)
+ (NSCursor *)contextualMenuCursor;
+ (NSCursor *)dragCopyCursor;
+ (NSCursor *)dragLinkCursor;
+ (NSCursor *)operationNotAllowedCursor;
@end

void NSOpenGLShell_setCursor(enum NSOpenGLShellCursor cursor) {
	switch (cursor) {
		case NSOpenGLShellCursor_arrow:
			[[NSCursor arrowCursor] set];
			break;
		case NSOpenGLShellCursor_IBeam:
			[[NSCursor IBeamCursor] set];
			break;
		case NSOpenGLShellCursor_crosshair:
			[[NSCursor crosshairCursor] set];
			break;
		case NSOpenGLShellCursor_closedHand:
			[[NSCursor closedHandCursor] set];
			break;
		case NSOpenGLShellCursor_openHand:
			[[NSCursor openHandCursor] set];
			break;
		case NSOpenGLShellCursor_pointingHand:
			[[NSCursor pointingHandCursor] set];
			break;
		case NSOpenGLShellCursor_resizeLeft:
			[[NSCursor resizeLeftCursor] set];
			break;
		case NSOpenGLShellCursor_resizeRight:
			[[NSCursor resizeRightCursor] set];
			break;
		case NSOpenGLShellCursor_resizeLeftRight:
			[[NSCursor resizeLeftRightCursor] set];
			break;
		case NSOpenGLShellCursor_resizeUp:
			[[NSCursor resizeUpCursor] set];
			break;
		case NSOpenGLShellCursor_resizeDown:
			[[NSCursor resizeDownCursor] set];
			break;
		case NSOpenGLShellCursor_resizeUpDown:
			[[NSCursor resizeUpDownCursor] set];
			break;
		case NSOpenGLShellCursor_disappearingItem:
			[[NSCursor disappearingItemCursor] set];
			break;
		case NSOpenGLShellCursor_contextualMenu:
			if ([NSCursor respondsToSelector: @selector(contextualMenuCursor)]) {
				[[NSCursor contextualMenuCursor] set];
			}
			break;
		case NSOpenGLShellCursor_dragCopy:
			if ([NSCursor respondsToSelector: @selector(dragCopyCursor)]) {
				[[NSCursor dragCopyCursor] set];
			}
			break;
		case NSOpenGLShellCursor_dragLink:
			if ([NSCursor respondsToSelector: @selector(dragLinkCursor)]) {
				[[NSCursor dragLinkCursor] set];
			}
			break;
		case NSOpenGLShellCursor_operationNotAllowed:
			if ([NSCursor respondsToSelector: @selector(operationNotAllowedCursor)]) {
				[[NSCursor operationNotAllowedCursor] set];
			}
			break;
	}
}

void NSOpenGLShell_getMainScreenDimensions(unsigned int * outWidth, unsigned int * outHeight) {
	NSRect bounds;
	
	bounds = [[NSScreen mainScreen] frame];
	if (outWidth != NULL) {
		*outWidth = bounds.size.width;
	}
	if (outHeight != NULL) {
		*outHeight = bounds.size.height;
	}
}
