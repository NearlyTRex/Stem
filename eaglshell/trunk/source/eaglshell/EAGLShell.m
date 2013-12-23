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

#include "EAGLShell.h"

#import <UIKit/UIKit.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <pthread.h>
#include <semaphore.h>

#import "eaglshell/EAGLShellApplication.h"
#include "shell/Shell.h"
#include "shell/ShellBatteryInfo.h"
#include "shell/ShellThreads.h"

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

@interface EAGLShellTimerTarget : NSObject {
	unsigned int timerID;
	bool repeat;
	void (* callback)(unsigned int timerID, void * context);
	void * context;
}
@end
@implementation EAGLShellTimerTarget
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

struct EAGLShellTimer {
	unsigned int id;
	NSTimer * timer;
	EAGLShellTimerTarget * timerTarget;
};

static bool isFullScreen = true;
static unsigned int nextTimerID;
static size_t timerCount;
static struct EAGLShellTimer * timers;

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
	if ([[[UIDevice currentDevice] systemVersion] compare: @"3.2" options: NSNumericSearch] != NSOrderedAscending) {
		[[UIApplication sharedApplication] setStatusBarHidden: fullScreen withAnimation: UIStatusBarAnimationSlide];
	} else {
		[[UIApplication sharedApplication] setStatusBarHidden: fullScreen animated: YES];
	}
	[(EAGLShellApplication *) [UIApplication sharedApplication] updateViewFrame];
	isFullScreen = fullScreen;
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

const char * Shell_getSupportPath() {
	static char supportPath[PATH_MAX];
	NSArray * paths;
	
	paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	strncpy(supportPath, [[paths objectAtIndex: 0] UTF8String], PATH_MAX);
	
	return supportPath;
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

void Shell_getMainScreenSize(unsigned int * outWidth, unsigned int * outHeight) {
	CGRect bounds;
	
	bounds = [UIScreen mainScreen].bounds;
	if (outWidth != NULL) {
		*outWidth = bounds.size.width;
	}
	if (outHeight != NULL) {
		*outHeight = bounds.size.height;
	}
}

unsigned int Shell_setTimer(double interval, bool repeat, void (* callback)(unsigned int timerID, void * context), void * context) {
	timers = realloc(timers, sizeof(struct EAGLShellTimer) * (timerCount + 1));
	timers[timerCount].id = nextTimerID++;
	timers[timerCount].timerTarget = [[EAGLShellTimerTarget alloc] initWithID: timers[timerCount].id repeat: repeat callback: callback context: context];
	timers[timerCount].timer = [NSTimer scheduledTimerWithTimeInterval: interval target: timers[timerCount].timerTarget selector: @selector(timer:) userInfo: nil repeats: repeat];
	return timers[timerCount++].id;
}

void Shell_cancelTimer(unsigned int timerID) {
	unsigned int timerIndex;
	
	for (timerIndex = 0; timerIndex < timerCount; timerIndex++) {
		if (timers[timerIndex].id == timerID) {
			[timers[timerIndex].timer invalidate];
			[timers[timerIndex].timerTarget release];
			timerCount--;
			for (; timerIndex < timerCount; timerIndex++) {
				timers[timerIndex] = timers[timerIndex + 1];
			}
			break;
		}
	}
}

void Shell_setCursorVisible(bool visible) {
}

void Shell_hideCursorUntilMouseMoves() {
}

void Shell_setCursor(int cursor) {
}

void Shell_setMouseDeltaMode(bool deltaMode) {
	extern bool g_mouseDeltaMode;
	
	g_mouseDeltaMode = deltaMode;
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
	return (void *) threadFunction(threadContext);
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
	pthread_exit((void *) statusCode);
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

enum EAGLShellOpenGLVersion EAGLShell_getOpenGLAPIVersion() {
	return [(EAGLShellApplication *) [UIApplication sharedApplication] chosenOpenGLVersion];
}

enum EAGLShellUIType EAGLShell_getUIType() {
	if ([[[UIDevice currentDevice] systemVersion] compare: @"3.2" options: NSNumericSearch] != NSOrderedAscending) {
		return [UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad ? EAGLShellUIType_iPad : EAGLShellUIType_iPhone;
	}
	return EAGLShellUIType_iPhone;
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

void EAGLShell_redirectStdoutToFile() {
	NSArray * directories;
	
	directories = [[NSFileManager defaultManager] URLsForDirectory: NSDocumentDirectory inDomains: NSAllDomainsMask];
	if ([directories count] > 0) {
		freopen([[[[directories objectAtIndex: 0] path] stringByAppendingPathComponent: @"stdout"] UTF8String], "a", stdout);
		setlinebuf(stdout);
	}
}
