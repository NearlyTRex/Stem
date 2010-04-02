#import <UIKit/UIKit.h>

extern int g_argc;
extern char ** g_argv;

@class EAGLView;

@interface EAGLShellApplication : UIApplication <UIApplicationDelegate, UITextFieldDelegate, UIAccelerometerDelegate> {
	UIWindow * window;
	EAGLView * view;
	UITextField * textField;
}

- (void) redisplayPosted;

- (void) showKeyboard;
- (void) hideKeyboard;
- (BOOL) isKeyboardVisible;

- (void) updateViewFrame;

- (void) enableAccelerometerWithInterval: (NSTimeInterval) interval;
- (void) disableAccelerometer;

@end
