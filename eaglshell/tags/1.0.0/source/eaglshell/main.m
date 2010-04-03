#import <UIKit/UIKit.h>
#import "eaglshell/EAGLShellApplication.h"

int main(int argc, char ** argv) {
	int exitCode;
	
	g_argc = argc;
	g_argv = argv;
	
	setlinebuf(stdout);
	
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	exitCode = UIApplicationMain(argc, argv, @"EAGLShellApplication", @"EAGLShellApplication");
	[pool release];
	return exitCode;
}
