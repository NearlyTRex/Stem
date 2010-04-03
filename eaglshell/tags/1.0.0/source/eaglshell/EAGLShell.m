#include "EAGLShell.h"

#import <UIKit/UIKit.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

#import "eaglshell/EAGLShellApplication.h"
#include "shell/Shell.h"
#include "shell/ShellBatteryInfo.h"

static bool isFullScreen = true;
bool mainLoopCalled = false;

void Shell_mainLoop() {
	mainLoopCalled = true;
}

void Shell_redisplay() {
	[(EAGLShellApplication *) [UIApplication sharedApplication] redisplayPosted];
}

bool Shell_isFullScreen() {
	return isFullScreen;
}

bool Shell_setFullScreen(bool fullScreen) {
	[[UIApplication sharedApplication] setStatusBarHidden: fullScreen animated: YES];
	[(EAGLShellApplication *) [UIApplication sharedApplication] updateViewFrame];
	isFullScreen = fullScreen;
	return true;
}

double Shell_getCurrentTime() {
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * timebaseInfo.numer / timebaseInfo.denom * 0.000000001;
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
	switch ([UIDevice currentDevice].batteryState) {
		case UIDeviceBatteryStateUnknown:
			return ShellBatteryState_unknown;
			
		case UIDeviceBatteryStateUnplugged:
			return ShellBatteryState_unplugged;
			
		case UIDeviceBatteryStateCharging:
			return ShellBatteryState_charging;
			
		case UIDeviceBatteryStateFull:
			return ShellBatteryState_full;
	}
	
	return ShellBatteryState_unknown;
}

float Shell_getBatteryLevel() {
	return [UIDevice currentDevice].batteryLevel;
}

enum EAGLShellOpenGLVersion EAGLShell_getOpenGLAPIVersion() {
	return [(EAGLShellApplication *) [UIApplication sharedApplication] chosenOpenGLVersion];
}

void EAGLShell_showKeyboard() {
	[(EAGLShellApplication *) [UIApplication sharedApplication] showKeyboard];
}

void EAGLShell_hideKeyboard() {
	[(EAGLShellApplication *) [UIApplication sharedApplication] hideKeyboard];
}

void EAGLShell_setOrientation(enum EAGLShellOrientation orientation) {
	UIInterfaceOrientation interfaceOrientation;
	BOOL keyboardWasShown;
	
	keyboardWasShown = [(EAGLShellApplication *) [UIApplication sharedApplication] isKeyboardVisible];
	switch (orientation) {
		case EAGLShellOrientation_deviceUpright:
			interfaceOrientation = UIInterfaceOrientationPortrait;
			break;
		case EAGLShellOrientation_deviceRotatedClockwise:
			interfaceOrientation = UIInterfaceOrientationLandscapeLeft;
			break;
		case EAGLShellOrientation_deviceRotatedCounterclockwise:
			interfaceOrientation = UIInterfaceOrientationLandscapeRight;
			break;
		case EAGLShellOrientation_deviceUpsideDown:
			interfaceOrientation = UIInterfaceOrientationPortraitUpsideDown;
			break;
		default:
			interfaceOrientation = UIInterfaceOrientationPortrait;
			break;
	}
	
	if (keyboardWasShown) {
		EAGLShell_hideKeyboard();
	}
	[[UIApplication sharedApplication] setStatusBarOrientation: interfaceOrientation animated: YES];
	if (keyboardWasShown) {
		EAGLShell_showKeyboard();
	}
}

void EAGLShell_setBatteryMonitoringEnabled(bool enabled) {
	[UIDevice currentDevice].batteryMonitoringEnabled = enabled;
}

void EAGLShell_setAccelerometerInterval(double interval) {
	if (interval <= 0.0) {
		[(EAGLShellApplication *) [UIApplication sharedApplication] disableAccelerometer];
	} else {
		[(EAGLShellApplication *) [UIApplication sharedApplication] enableAccelerometerWithInterval: interval];
	}
}
