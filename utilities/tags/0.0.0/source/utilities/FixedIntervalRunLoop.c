#include "utilities/FixedIntervalRunLoop.h"
#include "utilities/AutoFreePool.h"
#include <stdlib.h>

FixedIntervalRunLoop * FixedIntervalRunLoop_create(double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext) {
	FixedIntervalRunLoop * self;
	
	self = malloc(sizeof(FixedIntervalRunLoop));
	FixedIntervalRunLoop_init(self, timeFunction, stepInterval, stepCallback, stepContext);
	return self;
}

void FixedIntervalRunLoop_init(FixedIntervalRunLoop * self, double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext) {
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

void FixedIntervalRunLoop_dispose(void * selfPtr) {
}

void FixedIntervalRunLoop_run(void * selfPtr) {
	FixedIntervalRunLoop * self = selfPtr;
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

void FixedIntervalRunLoop_pause(void * selfPtr) {
	FixedIntervalRunLoop * self = selfPtr;
	
	if (!self->paused) {
		self->paused = true;
		self->pauseTime = self->timeFunction();
	}
}

void FixedIntervalRunLoop_resume(void * selfPtr) {
	FixedIntervalRunLoop * self = selfPtr;
	
	if (self->paused) {
		self->paused = false;
		self->lastTime += self->timeFunction() - self->pauseTime;
	}
}
