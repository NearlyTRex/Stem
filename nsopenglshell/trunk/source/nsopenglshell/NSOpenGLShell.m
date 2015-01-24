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

#include "nsopenglshell/NSOpenGLShell.h"

#import <Cocoa/Cocoa.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#import <IOKit/ps/IOPowerSources.h>
#import <IOKit/ps/IOPSKeys.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>

#import "nsopenglshell/NSOpenGLShellApplication.h"
#import "nsopenglshell/NSOpenGLShellView.h"
#include "shell/Shell.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellCallbacks.h"
#include "shell/ShellThreads.h"

#ifndef USE_NSTIMER
#define USE_NSTIMER 1
#endif

#if USE_NSTIMER

@interface NSOpenGLShellTimerTarget : NSObject {
	unsigned int timerID;
	bool repeat;
	void (* callback)(unsigned int timerID, void * context);
	void * context;
}
@end
@implementation NSOpenGLShellTimerTarget
- (id) initWithID: (unsigned int) inTimerID repeat: (bool) inRepeat callback: (void (*)(unsigned int timerID, void * context)) inCallback context: (void *) inContext {
	if ((self = [super init]) != nil) {
		timerID = inTimerID;
		repeat = inRepeat;
		callback = inCallback;
		context = inContext;
	}
	return self;
}

- (void) timer: (NSTimer *) timer {
	callback(timerID, context);
	if (!repeat) {
		Shell_cancelTimer(timerID);
	}
}
@end

struct NSOpenGLShellTimer {
	unsigned int id;
	NSTimer * timer;
	NSOpenGLShellTimerTarget * timerTarget;
};

#else

struct NSOpenGLShellTimer {
	double interval;
	double nextFireTime;
	bool repeat;
	unsigned int id;
	void (* callback)(unsigned int timerID, void * context);
	void * context;
};

#endif

bool mainLoopCalled = false;
static bool cursorHiddenByHide = false;
static unsigned int nextTimerID;
static size_t timerCount;
static struct NSOpenGLShellTimer * timers;
static unsigned int lastFullScreenDisplayIndex;

#if !USE_NSTIMER

static NSOpenGLShellView * view;

static void handleEvent(NSEvent * event) {
	switch ([event type]) {
		case NSFlagsChanged:
			[view flagsChanged: event];
			break;
			
		default:
			[NSApp sendEvent: event];
			break;
	}
}

void NSOpenGLShell_mainLoop() {
	NSAutoreleasePool * pool;
	NSEvent * event;
	NSDate * expiration;
	unsigned int timerIndex, timerIndex2;
	double currentTime;
	
	view = [(NSOpenGLShellApplication *) [NSApplication sharedApplication] view];
	for (;;) {
		pool = [[NSAutoreleasePool alloc] init];
		if (timerCount > 0 || [view redisplayWasPosted]) {
			expiration = nil;
		} else {
			expiration = [NSDate distantFuture];
		}
//#error This doesn't allow live window resizing! [[NSRunLoop currentRunLoop] currentMode] doesn't work for some reason (returns nil?)
		event = [NSApp nextEventMatchingMask: NSAnyEventMask untilDate: expiration inMode: NSDefaultRunLoopMode dequeue: YES];
		while (event != nil) {
			handleEvent(event);
			event = [NSApp nextEventMatchingMask: NSAnyEventMask untilDate: nil inMode: NSDefaultRunLoopMode dequeue: YES];
		}
		
		currentTime = Shell_getCurrentTime();
		for (timerIndex = 0; timerIndex < timerCount; timerIndex++) {
			if (currentTime >= timers[timerIndex].nextFireTime) {
				timers[timerIndex].callback(timers[timerIndex].id, timers[timerIndex].context);
				if (timers[timerIndex].repeat) {
					timers[timerIndex].nextFireTime += timers[timerIndex].interval;
				} else {
					timerCount--;
					for (timerIndex2 = timerIndex; timerIndex2 < timerCount; timerIndex2++) {
						timers[timerIndex2] = timers[timerIndex2 + 1];
					}
					timerIndex--;
				}
			}
		}
		
		if ([view redisplayWasPosted]) {
			[view draw];
		}
		
		[pool release];
	}
}

#endif

void Shell_mainLoop() {
	mainLoopCalled = true;
}

void Shell_redisplay() {
	[[(NSOpenGLShellApplication *) [NSApplication sharedApplication] view] redisplay];
}

bool Shell_isFullScreen() {
	return [[(NSOpenGLShellApplication *) [NSApplication sharedApplication] view] isInFullScreenMode];
}

bool Shell_enterFullScreen(unsigned int displayIndex) {
	if (displayIndex >= [[NSScreen screens] count]) {
		displayIndex = Shell_getDisplayIndexFromWindow();
	}
	lastFullScreenDisplayIndex = displayIndex;
	return [[(NSOpenGLShellApplication *) [NSApplication sharedApplication] view] enterFullScreen: [[NSScreen screens] objectAtIndex: displayIndex]];
}

void Shell_exitFullScreen() {
	[[(NSOpenGLShellApplication *) [NSApplication sharedApplication] view] exitFullScreen];
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

const char * Shell_getSupportPath(const char * subdirectory) {
	static char supportPath[PATH_MAX];
	NSArray * paths;
	
	paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	if (subdirectory == NULL) {
		strncpy(supportPath, [[paths objectAtIndex: 0] UTF8String], PATH_MAX);
	} else {
		snprintf(supportPath, PATH_MAX, "%s/%s", [[paths objectAtIndex: 0] UTF8String], subdirectory);
	}
	mkdir(supportPath, 0777);
	
	return supportPath;
}

void Shell_openURL(const char * url) {
	[[NSWorkspace sharedWorkspace] openURL: [NSURL URLWithString: [NSString stringWithUTF8String: url]]];
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

unsigned int Shell_getDisplayCount() {
	return [[NSScreen screens] count];
}

unsigned int Shell_getDisplayIndexFromWindow() {
	NSScreen * screen = [[(NSOpenGLShellApplication *) [NSApplication sharedApplication] window] screen];
	NSUInteger screenIndex = [[NSScreen screens] indexOfObject: screen];
	
	if (screenIndex == NSNotFound) {
		screenIndex = [[NSScreen screens] indexOfObject: [NSScreen mainScreen]];
		if (screenIndex == NSNotFound) {
			return 0;
		}
	}
	return screenIndex;
}

void Shell_getDisplayBounds(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	NSArray * screens = [NSScreen screens];
	
	if (displayIndex < [screens count]) {
		NSRect bounds = [[screens objectAtIndex: displayIndex] frame];
		bounds.origin.y = [[NSScreen mainScreen] frame].size.height - bounds.origin.y - bounds.size.height;
		if (outOffsetX != NULL) {
			*outOffsetX = bounds.origin.x;
		}
		if (outOffsetY != NULL) {
			*outOffsetY = bounds.origin.y;
		}
		if (outWidth != NULL) {
			*outWidth = bounds.size.width;
		}
		if (outHeight != NULL) {
			*outHeight = bounds.size.height;
		}
	}
}

void Shell_getSafeWindowRect(unsigned int displayIndex, int * outOffsetX, int * outOffsetY, unsigned int * outWidth, unsigned int * outHeight) {
	NSArray * screens = [NSScreen screens];
	
	if (displayIndex < [screens count]) {
		NSRect bounds = [[screens objectAtIndex: displayIndex] visibleFrame];
		bounds = [NSWindow contentRectForFrameRect: bounds styleMask: NSTitledWindowMask];
		bounds.origin.y = [[NSScreen mainScreen] frame].size.height - bounds.origin.y - bounds.size.height;
		if (outOffsetX != NULL) {
			*outOffsetX = bounds.origin.x;
		}
		if (outOffsetY != NULL) {
			*outOffsetY = bounds.origin.y;
		}
		if (outWidth != NULL) {
			*outWidth = bounds.size.width;
		}
		if (outHeight != NULL) {
			*outHeight = bounds.size.height;
		}
	}
}

unsigned int Shell_setTimer(double interval, bool repeat, void (* callback)(unsigned int timerID, void * context), void * context) {
	timers = realloc(timers, sizeof(struct NSOpenGLShellTimer) * (timerCount + 1));
	timers[timerCount].id = nextTimerID++;
#if USE_NSTIMER
	timers[timerCount].timerTarget = [[NSOpenGLShellTimerTarget alloc] initWithID: timers[timerCount].id repeat: repeat callback: callback context: context];
	timers[timerCount].timer = [NSTimer scheduledTimerWithTimeInterval: interval target: timers[timerCount].timerTarget selector: @selector(timer:) userInfo: nil repeats: repeat];
#else
	timers[timerCount].interval = interval;
	timers[timerCount].nextFireTime = Shell_getCurrentTime() + interval;
	timers[timerCount].repeat = repeat;
	timers[timerCount].callback = callback;
	timers[timerCount].context = context;
#endif
	return timers[timerCount++].id;
}

void Shell_cancelTimer(unsigned int timerID) {
	unsigned int timerIndex;
	
	for (timerIndex = 0; timerIndex < timerCount; timerIndex++) {
		if (timers[timerIndex].id == timerID) {
#if USE_NSTIMER
			[timers[timerIndex].timer invalidate];
			[timers[timerIndex].timerTarget release];
#endif
			timerCount--;
			for (; timerIndex < timerCount; timerIndex++) {
				timers[timerIndex] = timers[timerIndex + 1];
			}
			break;
		}
	}
}

void Shell_setCursorVisible(bool visible) {
	if (visible) {
		if (cursorHiddenByHide) {
			cursorHiddenByHide = false;
			if (!g_mouseDeltaMode) {
				[NSCursor unhide];
			}
		}
	} else {
		if (!cursorHiddenByHide) {
			cursorHiddenByHide = true;
			[NSCursor hide];
		}
	}
}

void Shell_hideCursorUntilMouseMoves() {
	[NSCursor setHiddenUntilMouseMoves: YES];
}

@interface NSCursor (MacOSX10_6Cursors)
+ (NSCursor *)contextualMenuCursor;
+ (NSCursor *)dragCopyCursor;
+ (NSCursor *)dragLinkCursor;
+ (NSCursor *)operationNotAllowedCursor;
@end

void Shell_setCursor(int cursor) {
	switch (cursor) {
		case ShellCursor_arrow:
			[[NSCursor arrowCursor] set];
			break;
		case ShellCursor_iBeam:
			[[NSCursor IBeamCursor] set];
			break;
		case ShellCursor_crosshair:
			[[NSCursor crosshairCursor] set];
			break;
		case ShellCursor_hand:
			[[NSCursor pointingHandCursor] set];
			break;
		case NSOpenGLShellCursor_closedHand:
			[[NSCursor closedHandCursor] set];
			break;
		case NSOpenGLShellCursor_openHand:
			[[NSCursor openHandCursor] set];
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

void Shell_setMouseDeltaMode(bool deltaMode) {
	if (g_mouseDeltaMode != deltaMode) {
		g_mouseDeltaMode = deltaMode;
		if (!cursorHiddenByHide) {
			if (deltaMode) {
				[NSCursor hide];
			} else {
				[NSCursor unhide];
			}
		}
		CGAssociateMouseAndMouseCursorPosition(!deltaMode);
	}
}

struct threadFuncInvocation {
	int (* threadFunction)(void * context);
	void * context;
};

static void * callThreadFunc(void * context) {
	struct threadFuncInvocation * invocation = context;
	int (* threadFunction)(void * context);
	void * threadContext;
	
	threadFunction  = invocation->threadFunction;
	threadContext = invocation->context;
	free(invocation);
	return (void *) (ptrdiff_t) threadFunction(threadContext);
}

ShellThread Shell_createThread(int (* threadFunction)(void * context), void * context) {
	pthread_t thread;
	struct threadFuncInvocation * invocation;
	
	invocation = malloc(sizeof(struct threadFuncInvocation));
	invocation->threadFunction = threadFunction;
	invocation->context = context;
	pthread_create(&thread, NULL, callThreadFunc, invocation);
	return (ShellThread) thread;
}

void Shell_detachThread(ShellThread thread) {
	pthread_detach((pthread_t) thread);
}

int Shell_joinThread(ShellThread thread) {
	int status;
	void * returnValue;
	
	status = pthread_join((pthread_t) thread, &returnValue);
	if (status == 0) {
		return (int) returnValue;
	}
	return status;
}

void Shell_exitThread(int statusCode) {
	pthread_exit((void *) (ptrdiff_t) statusCode);
}

ShellThread Shell_getCurrentThread() {
	return (ShellThread) pthread_self();
}

ShellMutex Shell_createMutex() {
	pthread_mutex_t * mutex;
	pthread_mutexattr_t recursiveLock;

	pthread_mutexattr_init(&recursiveLock);
	pthread_mutexattr_settype(&recursiveLock, PTHREAD_MUTEX_RECURSIVE);
	mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, &recursiveLock);
	pthread_mutexattr_destroy(&recursiveLock);
	return (ShellMutex) mutex;
}

void Shell_disposeMutex(ShellMutex mutex) {
	pthread_mutex_destroy((pthread_mutex_t *) mutex);
	free(mutex);
}

void Shell_lockMutex(ShellMutex mutex) {
	pthread_mutex_lock((pthread_mutex_t *) mutex);
}

bool Shell_tryLockMutex(ShellMutex mutex) {
	return !pthread_mutex_trylock((pthread_mutex_t *) mutex);
}

void Shell_unlockMutex(ShellMutex mutex) {
	pthread_mutex_unlock((pthread_mutex_t *) mutex);
}

ShellSemaphore Shell_createSemaphore(unsigned int value) {
	sem_t * semaphore;
	
	semaphore = malloc(sizeof(sem_t));
	sem_init(semaphore, 0, value > SEM_VALUE_MAX ? SEM_VALUE_MAX : value);
	return (ShellSemaphore) semaphore;
}

void Shell_disposeSemaphore(ShellSemaphore semaphore) {
	sem_destroy((sem_t *) semaphore);
	free(semaphore);
}

void Shell_postSemaphore(ShellSemaphore semaphore) {
	sem_post((sem_t *) semaphore);
}

void Shell_waitSemaphore(ShellSemaphore semaphore) {
	sem_wait((sem_t *) semaphore);
}

bool Shell_tryWaitSemaphore(ShellSemaphore semaphore) {
	return !sem_trywait((sem_t *) semaphore);
}

void Shell_setVSync(bool sync, bool fullscreen) {
	[[(NSOpenGLShellApplication *) [NSApplication sharedApplication] view] setVSync: sync forFullscreen: fullscreen];
}

void Shell_systemBeep() {
	NSBeep();
}

bool Shell_openFileDialog(const char * basePath, char * outFilePath, unsigned int maxLength) {
	bool wasFullScreen = Shell_isFullScreen();
	if (wasFullScreen) {
		Shell_exitFullScreen();
	}
	
	NSOpenPanel * openPanel = [NSOpenPanel openPanel];
	if (basePath != NULL) {
		[openPanel setDirectoryURL: [NSURL fileURLWithPath: [NSString stringWithUTF8String: basePath]]];
	}
	
	NSInteger result = [openPanel runModal];
	if (wasFullScreen) {
		Shell_enterFullScreen(lastFullScreenDisplayIndex);
	}
	if (result == NSFileHandlingPanelOKButton) {
		strncpy(outFilePath, [[[openPanel URLs] objectAtIndex: 0] fileSystemRepresentation], maxLength);
		return true;
	}
	return false;
}

bool Shell_saveFileDialog(const char * basePath, const char * baseName, char * outFilePath, unsigned int maxLength) {
	bool wasFullScreen = Shell_isFullScreen();
	if (wasFullScreen) {
		Shell_exitFullScreen();
	}
	
	NSSavePanel * savePanel = [NSSavePanel savePanel];
	if ([savePanel respondsToSelector: @selector(setShowsTagField:)]) {
		[savePanel setShowsTagField: NO];
	}
	if (basePath != NULL) {
		[savePanel setDirectoryURL: [NSURL fileURLWithPath: [NSString stringWithUTF8String: basePath]]];
	}
	if (baseName != NULL) {
		[savePanel setNameFieldStringValue: [NSString stringWithUTF8String: baseName]];
	}
	
	NSInteger result = [savePanel runModal];
	if (wasFullScreen) {
		Shell_enterFullScreen(lastFullScreenDisplayIndex);
	}
	if (result == NSFileHandlingPanelOKButton) {
		strncpy(outFilePath, [[savePanel URL] fileSystemRepresentation], maxLength);
		return true;
	}
	return false;
}

float NSOpenGLShell_getDisplayScaleFactor(unsigned int displayIndex) {
	if (![NSScreen instancesRespondToSelector: @selector(backingScaleFactor)]) {
		return 1.0f;
	}
	if (displayIndex >= [[NSScreen screens] count]) {
		displayIndex = 0;
	}
	return [[[NSScreen screens] objectAtIndex: displayIndex] backingScaleFactor];
}
