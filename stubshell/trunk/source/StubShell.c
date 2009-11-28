#include "shell/Shell.h"

#if defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

void Shell_mainLoop() {
}

void Shell_redisplay() {
}

double Shell_getCurrentTime() {
#if defined(__APPLE__)
	static mach_timebase_info_data_t timebaseInfo;
	
	if (timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}
	return mach_absolute_time() * timebaseInfo.numer / timebaseInfo.denom / 1000000000.0;
#elif defined(WIN32)
	//QueryPerformanceCounter
	return 0;
#elif defined(__linux)
	//clock_gettime(CLOCK_MONOTONIC)
	return 0;
#else
#error Unimplemented target OS
#endif
}

const char * Shell_getResourcePath() {
	return "";
}
