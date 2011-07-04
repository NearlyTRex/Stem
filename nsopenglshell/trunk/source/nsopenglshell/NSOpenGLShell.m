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

#import "nsopenglshell/NSOpenGLShellApplication.h"
#include "shell/Shell.h"
#include "shell/ShellBatteryInfo.h"

bool mainLoopCalled = false;

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
	// TODO (see http://developer.apple.com/library/mac/#samplecode/BatteryInfo/Listings/BatteryInfoPlugIn_m.html)
	return ShellBatteryState_unknown;
}

float Shell_getBatteryLevel() {
	// TODO (see http://developer.apple.com/library/mac/#samplecode/BatteryInfo/Listings/BatteryInfoPlugIn_m.html)
	return 1.0f;
}
