#ifndef __FIXED_INTERVAL_RUN_LOOP_H__
#define __FIXED_INTERVAL_RUN_LOOP_H__

#include <stdbool.h>

typedef struct FixedIntervalRunLoop FixedIntervalRunLoop;
typedef void (* FixedIntervalRunLoopCallback)(void * context);

#define FixedIntervalRunLoop_structContents \
	double (* timeFunction)(); \
	double stepInterval; \
	FixedIntervalRunLoopCallback stepCallback; \
	void * stepContext; \
	double lastTime; \
	double slop; \
	bool paused; \
	double pauseTime; \
	\
	void (* dispose)(void * self); \
	void (* run)(void * self); \
	void (* pause)(void * self); \
	void (* resume)(void * self);

struct FixedIntervalRunLoop {
	FixedIntervalRunLoop_structContents
};

// timeFunction is a pointer to a function that will return the current time in seconds when called
FixedIntervalRunLoop * FixedIntervalRunLoop_create(double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext);
void FixedIntervalRunLoop_init(FixedIntervalRunLoop * self, double (* timeFunction)(), double stepInterval, FixedIntervalRunLoopCallback stepCallback, void * stepContext);

void FixedIntervalRunLoop_dispose(void * selfPtr);
void FixedIntervalRunLoop_run(void * selfPtr);
void FixedIntervalRunLoop_pause(void * selfPtr);
void FixedIntervalRunLoop_resume(void * selfPtr);

#endif
