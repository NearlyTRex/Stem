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

#ifndef __SHELL_THREADS_H__
#define __SHELL_THREADS_H__

#include <stdbool.h>

typedef void * ShellThread;
typedef void * ShellMutex;
typedef void * ShellSemaphore;

/** Creates a new preemptive thread and invokes threadFunction from it, passing context as its
    only argument. The thread function's return value will be returned to the caller of
    Shell_joinThread(). */
ShellThread Shell_createThread(int (* threadFunction)(void * context), void * context);

/** Exits the current thread, return statusCode to callers of Shell_joinThread(). */
void Shell_exitThread(int statusCode);

/** Suspends the current thread until the specified thread has completed, returning the thread's
    exit code. */
int Shell_joinThread(ShellThread thread);

/** Returns the thread in which the caller is executing. */
ShellThread Shell_getCurrentThread();

/** Initializes and returns a new mutex object. The caller is responsible for calling
    Shell_disposeMutex() when the mutex is no longer needed. */
ShellMutex Shell_createMutex();

/** Disposes the specified mutex, freeing any resources allocated to it. */
void Shell_disposeMutex(ShellMutex mutex);

/** Blocks the calling thread until a lock can be acquired on the specified mutex. Must be
    balanced by a call to Shell_unlockMutex(). */
void Shell_lockMutex(ShellMutex mutex);

/** Attempts to lock the specified mutex without blocking the calling thread, returning true
    if the lock could be acquired, or false if it could not. If true is returned, must be
    balanced by a call to Shell_unlockMutex(). */
bool Shell_tryLockMutex(ShellMutex mutex);

/** Releases the current thread's lock on the specified mutex. Must be called only after
    the specified mutex has been successfully locked by a call to Shell_lockMutex() or
    Shell_tryLockMutex(). */
void Shell_unlockMutex(ShellMutex mutex);

/** Initializes and returns a new semaphore object with the specified value. The caller is
    responsible for calling Shell_disposeSemaphore() when the semaphore is no longer needed. */
ShellSemaphore Shell_createSemaphore(unsigned int value);

/** Disposes the specified semaphore, freeing any resources allocated to it. */
void Shell_disposeSemaphore(ShellSemaphore semaphore);

/** Increments the semaphore's value by 1, allowing any threads blocks on Shell_waitSemaphore()
    to become unblocked. */
void Shell_postSemaphore(ShellSemaphore semaphore);

/** Decrements the semaphore's value by 1, blocking if the value is 0. */
void Shell_waitSemaphore(ShellSemaphore semaphore);

/** Attempts to decrement the semaphore's value by 1, returning true if successful, or false
    if it would be necessary to block. */
bool Shell_tryWaitSemaphore(ShellSemaphore semaphore);

#endif
