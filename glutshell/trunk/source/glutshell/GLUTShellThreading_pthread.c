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

#include "shell/ShellThreads.h"
#include <pthread.h>
#include <stdlib.h>

struct threadFuncInvocation {
	int (* threadFunction)(void * context);
	void * context;
};

static void * callThreadFunc(void * context) {
	struct threadFuncInvocation * invocation = context;
	int (* threadFunction)(void * context);
	void * threadContext;
	
	threadFunction  = invocation->threadFunction;
	threadContext = invocation->context;
	free(invocation);
	return (void *) threadFunction(threadContext);
}

ShellThread Shell_createThread(int (* threadFunction)(void * context), void * context) {
	pthread_t thread;
	struct threadFuncInvocation * invocation;
	
	invocation = malloc(sizeof(struct threadFuncInvocation));
	invocation->threadFunction = threadFunction;
	invocation->context = context;
	pthread_create(&thread, NULL, callThreadFunc, invocation);
	return thread;
}

void Shell_exitThread(int statusCode) {
	pthread_exit((void *) statusCode);
}

int Shell_joinThread(ShellThread thread) {
	int status;
	void * returnValue;
	
	status = pthread_join(thread, &returnValue);
	if (status == 0) {
		return (int) returnValue;
	}
	return status;
}

ShellThread Shell_getCurrentThread() {
	return pthread_self();
}

ShellMutex Shell_createMutex() {
	pthread_mutex_t * mutex;
	pthread_mutexattr_t recursiveLock;

	pthread_mutexattr_init(&recursiveLock);
	pthread_mutexattr_settype(&recursiveLock, PTHREAD_MUTEX_RECURSIVE);
	mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);
	pthread_mutexattr_destroy(&recursiveLock);
	return mutex;
}

void Shell_disposeMutex(ShellMutex mutex) {
	pthread_mutex_destroy(mutex);
	free(mutex);
}

void Shell_lockMutex(ShellMutex mutex) {
	pthread_mutex_lock(mutex);
}

bool Shell_tryLockMutex(ShellMutex mutex) {
	return !pthread_mutex_trylock(mutex);
}

void Shell_unlockMutex(ShellMutex mutex) {
	pthread_mutex_unlock(mutex);
}

ShellSemaphore Shell_createSemaphore(unsigned int value) {
	sem_t * semaphore;
	
	semaphore = malloc(sizeof(sem_t));
	sem_init(semaphore, 0, value > SEM_VALUE_MAX ? SEM_VALUE_MAX : value);
	return semaphore;
}

void Shell_disposeSemaphore(ShellSemaphore semaphore) {
	sem_destroy(semaphore);
	free(semaphore);
}

void Shell_postSemaphore(ShellSemaphore semaphore) {
	sem_post(semaphore);
}

void Shell_waitSemaphore(ShellSemaphore semaphore) {
	sem_wait(semaphore);
}

bool Shell_tryWaitSemaphore(ShellSemaphore semaphore) {
	return !sem_trywait(semaphore);
}
