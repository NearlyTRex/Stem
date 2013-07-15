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

#include "utilities/FixedIntervalRunLoop.h"
#include "utilities/AutoFreePool.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

FixedIntervalRunLoop * FixedIntervalRunLoop_create(double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext) {
	stemobject_create_implementation(FixedIntervalRunLoop, init, timeFunction, stepInterval, stepCallback, stepContext)
}

void FixedIntervalRunLoop_init(FixedIntervalRunLoop * self, double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext) {
	call_super(init, self);
	
	self->timeFunction = timeFunction;
	self->stepInterval = stepInterval;
	self->stepCallback = stepCallback;
	self->stepContext = stepContext;
	self->lastTime = self->timeFunction();
	self->slop = 0.0;
	self->paused = false;
	
	self->dispose = FixedIntervalRunLoop_dispose;
	self->run = FixedIntervalRunLoop_run;
	self->pause = FixedIntervalRunLoop_pause;
	self->resume = FixedIntervalRunLoop_resume;
}

void FixedIntervalRunLoop_dispose(FixedIntervalRunLoop * self) {
	call_super(dispose, self);
}

void FixedIntervalRunLoop_run(FixedIntervalRunLoop * self) {
	double currentTime, interval;
	
	if (self->paused) {
		return;
	}
	currentTime = self->timeFunction();
	interval = (currentTime - self->lastTime) + self->slop;
	
	while (interval >= self->stepInterval) {
		self->stepCallback(self->stepContext);
		interval -= self->stepInterval;
	}
	self->slop = interval;
	self->lastTime = currentTime;
	
	AutoFreePool_empty();
}

void FixedIntervalRunLoop_pause(FixedIntervalRunLoop * self) {
	if (!self->paused) {
		self->paused = true;
		self->pauseTime = self->timeFunction();
	}
}

void FixedIntervalRunLoop_resume(FixedIntervalRunLoop * self) {
	if (self->paused) {
		self->paused = false;
		self->lastTime += self->timeFunction() - self->pauseTime;
	}
}
