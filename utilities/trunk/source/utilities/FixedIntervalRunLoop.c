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

#include "utilities/FixedIntervalRunLoop.h"
#include "utilities/AutoFreePool.h"
#include <stdlib.h>

#define SUPERCLASS StemObject

FixedIntervalRunLoop * FixedIntervalRunLoop_create(double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext) {
	stemobject_create_implementation(FixedIntervalRunLoop, init, timeFunction, stepInterval, stepCallback, stepContext)
}

bool FixedIntervalRunLoop_init(FixedIntervalRunLoop * self, double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext) {
	call_super(init, self);
	
	self->timeFunction = timeFunction;
	self->stepInterval = stepInterval;
	self->stepCallback = stepCallback;
	self->stepContext = stepContext;
	self->lastTime = self->timeFunction();
	self->slop = 0.0;
	self->tolerance = 0.0;
	self->paused = false;
	self->private_ivar(disposedWhileRunning) = NULL;
	self->dispose = FixedIntervalRunLoop_dispose;
	return true;
}

void FixedIntervalRunLoop_dispose(FixedIntervalRunLoop * self) {
	if (self->private_ivar(disposedWhileRunning) != NULL) {
		*self->private_ivar(disposedWhileRunning) = true;
	}
	call_super(dispose, self);
}

unsigned int FixedIntervalRunLoop_run(FixedIntervalRunLoop * self) {
	double currentTime, interval;
	unsigned int calls = 0;
	bool disposedWhileRunning = false;
	
	if (self->paused) {
		return 0;
	}
	currentTime = self->timeFunction();
	interval = (currentTime - self->lastTime) + self->slop;
	
	self->private_ivar(disposedWhileRunning) = &disposedWhileRunning;
	while (interval >= self->stepInterval) {
		if (calls == 1 && interval <= self->stepInterval + self->tolerance) {
			break;
		}
		self->stepCallback(self->stepContext);
		calls++;
		if (disposedWhileRunning) {
			return calls;
		}
		interval -= self->stepInterval;
	}
	if (calls == 0 && interval >= self->stepInterval - self->tolerance) {
		self->stepCallback(self->stepContext);
		calls++;
		if (disposedWhileRunning) {
			return calls;
		}
		interval -= self->stepInterval;
	}
	self->slop = interval;
	self->lastTime = currentTime;
	self->private_ivar(disposedWhileRunning) = NULL;
	
	AutoFreePool_empty();
	
	return calls;
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

void FixedIntervalRunLoop_setTolerance(FixedIntervalRunLoop * self, double tolerance) {
	self->tolerance = tolerance;
}
