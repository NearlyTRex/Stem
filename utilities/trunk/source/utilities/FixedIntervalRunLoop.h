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

#ifndef __FIXED_INTERVAL_RUN_LOOP_H__
#define __FIXED_INTERVAL_RUN_LOOP_H__

#include "stemobject/StemObject.h"

typedef struct FixedIntervalRunLoop FixedIntervalRunLoop;
typedef void (* FixedIntervalRunLoopCallback)(void * context);

#define FixedIntervalRunLoop_structContents(self_type) \
	StemObject_structContents(self_type) \
	\
	double (* timeFunction)(); \
	double stepInterval; \
	FixedIntervalRunLoopCallback stepCallback; \
	void * stepContext; \
	double timeScale; \
	double tolerance; \
	double slop; \
	bool paused; \
	double pauseTime; \
	double protected_ivar(lastTime); \
	bool * private_ivar(disposedWhileRunning);

stemobject_struct_definition(FixedIntervalRunLoop)

// timeFunction is a pointer to a function that will return the current time in seconds when called
FixedIntervalRunLoop * FixedIntervalRunLoop_create(double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext);
bool FixedIntervalRunLoop_init(FixedIntervalRunLoop * self, double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext);
void FixedIntervalRunLoop_dispose(FixedIntervalRunLoop * self);

// Invokes stepCallback zero or more times based on the time interval since the last call, as measured by timeFunction. Returns the number of times stepCallback was invoked.
// Calls AutoFreePool_empty() before returning.
// Special note: It is safe to dispose a run loop from a callback invoked by FixedIntervalRunLoop_run().
unsigned int FixedIntervalRunLoop_run(FixedIntervalRunLoop * self);

// Suspends time measurement for the run loop. Calls to FixedIntervalRunLoop_run while paused will behave as if time is not advancing.
// Calling this function while the run loop is paused has no effect.
void FixedIntervalRunLoop_pause(FixedIntervalRunLoop * self);

// Resumes time measurement for the run loop. Subsequent calls to FixedIntervalRunLoop_run will behave as though no actual time passed while the run loop was paused.
// Calling this function while the run loop is not paused has no effect.
void FixedIntervalRunLoop_resume(FixedIntervalRunLoop * self);

// Sets a finite amount of time deviation allowed between stepCallback invocations, used to relieve temporal aliasing. Defaults to 0.
// If set, FixedIntervalRunLoop_run will deviate by up to the specified amount of time in either direction in an attempt to invoke stepCallback exactly once per call.
// It is recommended to set tolerance to a value less than half of the run loop's stepInterval. Behavior is undefined for negative values.
void FixedIntervalRunLoop_setTolerance(FixedIntervalRunLoop * self, double tolerance);

// Sets a mutiplier value to be applied to measured time intervals the next time run() is called. Higher values speed up time, causing
// the run callback to be called more often. A value of 2.0 will run at double speed; a value of 0.5 will run at half speed.
// Behavior is undefined for negative values. Default 1.0.
void FixedIntervalRunLoop_setTimeScale(FixedIntervalRunLoop * self, double multiplier);

#endif
