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

#ifndef __FIXED_INTERVAL_RUN_LOOP_H__
#define __FIXED_INTERVAL_RUN_LOOP_H__

#include "stemobject/StemObject.h"

typedef struct FixedIntervalRunLoop FixedIntervalRunLoop;
typedef void (* FixedIntervalRunLoopCallback)(void * context);

#define FixedIntervalRunLoop_structContents \
	StemObject_structContents \
	\
	double (* timeFunction)(); \
	double stepInterval; \
	FixedIntervalRunLoopCallback stepCallback; \
	void * stepContext; \
	double lastTime; \
	double slop; \
	bool paused; \
	double pauseTime; \
	\
	void (* run)(compat_type(FixedIntervalRunLoop *) self); \
	void (* pause)(compat_type(FixedIntervalRunLoop *) self); \
	void (* resume)(compat_type(FixedIntervalRunLoop *) self);

struct FixedIntervalRunLoop {
	FixedIntervalRunLoop_structContents
};

// timeFunction is a pointer to a function that will return the current time in seconds when called
FixedIntervalRunLoop * FixedIntervalRunLoop_create(double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext);
void FixedIntervalRunLoop_init(compat_type(FixedIntervalRunLoop *) selfPtr, double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext);

void FixedIntervalRunLoop_dispose(compat_type(FixedIntervalRunLoop *) selfPtr);
void FixedIntervalRunLoop_run(compat_type(FixedIntervalRunLoop *) selfPtr);
void FixedIntervalRunLoop_pause(compat_type(FixedIntervalRunLoop *) selfPtr);
void FixedIntervalRunLoop_resume(compat_type(FixedIntervalRunLoop *) selfPtr);

#endif
